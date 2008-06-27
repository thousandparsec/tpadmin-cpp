/*  Session class
 *
 *  Copyright (C) 2008 Aaron Mavrinac and the Thousand Parsec Project
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <sstream>
#include <string.h>
#include <boost/bind.hpp>

#include <tprl/console.h>
#include <tprl/rlcommand.h>

#include <tpproto/adminlayer.h>
#include <tpproto/simpleeventloop.h>
#include <tpproto/commanddesccache.h>

#include "consolelogger.h"
#include "clientasl.h"

#include "session.h"

class QuitCommand : public tprl::RLCommand
{
  public:
    QuitCommand() : tprl::RLCommand()
    {
        name = "quit";
        help = "Quit tpadmin-cpp administration client.";
    }
    
    void action(const std::string & cmdline)
    {
        if(Session::getSession()->getAdminLayer()->getStatus() != TPProto::asDisconnected)
            Session::getSession()->getAdminLayer()->disconnect();
        Session::getSession()->stopMainLoop();
    }
};

class OpenCommand : public tprl::RLCommand
{
  public:
    OpenCommand() : tprl::RLCommand()
    {
        name = "open";
        help = "Open an administration connection to a TP server.";
    }

    void action(const std::string & cmdline)
    {
        std::ostringstream msg;
        if(Session::getSession()->getAdminLayer()->getStatus() == TPProto::asDisconnected){
            if(Session::getSession()->getAdminLayer()->connect(cmdline)){
                msg << "Connected ok, status: " << Session::getSession()->getAdminLayer()->getStatus();
                Session::getSession()->getLogger()->debug(msg.str().c_str());
            }
        }else{
            Session::getSession()->getLogger()->warning("Already connected to a server.");
        }
    }
};

class LoginCommand : public tprl::RLCommand
{
  public:
    LoginCommand() : tprl::RLCommand()
    {
        name = "login";
        help = "Log in to administration on a TP server.";
    }

    void action(const std::string & cmdline)
    {
        std::ostringstream msg;
        if(Session::getSession()->getAdminLayer()->getStatus() == TPProto::asConnected){
            size_t p = cmdline.find(' ');
            if(Session::getSession()->getAdminLayer()->login(cmdline.substr(0, p), cmdline.substr(p + 1))){
                msg << "Login ok, status: " << Session::getSession()->getAdminLayer()->getStatus();
                Session::getSession()->getLogger()->debug(msg.str().c_str());
            }
        }else{
            Session::getSession()->getLogger()->warning("Not connected or already logged in.");
        }
    }
};

class CloseCommand : public tprl::RLCommand
{
  public:
    CloseCommand() : tprl::RLCommand()
    {
        name = "close";
        help = "Close the current administration connection.";
    }

    void action(const std::string & cmdline)
    {
        if(Session::getSession()->getAdminLayer()->getStatus() != TPProto::asDisconnected)
            Session::getSession()->getAdminLayer()->disconnect();
    }
};

Session * Session::myInstance = NULL;

Session * Session::getSession()
{
    if(myInstance == NULL)
        myInstance = new Session();
    return myInstance;
}

void Session::start()
{
    if(console == NULL)
    {
        console = tprl::Console::getConsole();
        console->setCatchSignals(false);
        console->setUseHistory(true);
        console->setCommandSet(&commands);
        console->setPrompt("tpadmin-cpp> ");
        console->readLine_nb_start();
    }

    eventloop->setTimer(1, boost::bind(&Session::mainLoop, this));
    eventloop->runEventLoop();
}

void Session::stop()
{
    console->readLine_nb_stop();

    if(layer->getStatus() != TPProto::asDisconnected)
        layer->disconnect();

    eventloop->endEventLoop();
}

void Session::mainLoop()
{
    while(!halt)
    {
        console->readLine_nb_inputReady();
    }

    stop();
}

void Session::stopMainLoop()
{
    halt = true;
}

void Session::getCommands()
{
    layer->getCommandDescCache()->requestCommandTypes(boost::bind(&Session::receiveCommands, this, _1));
}

void Session::receiveCommands(std::set<uint32_t> ids)
{
}

void Session::addCommand(tprl::RLCommand * command)
{
    commands.insert(command);
}

TPProto::AdminLayer * Session::getAdminLayer() const
{
    return layer;
}

tprl::Console * Session::getConsole() const
{
    return console;
}

ConsoleLogger * Session::getLogger() const
{
    return logger;
}

Session::Session()
{
    logger = new ConsoleLogger();

    eventloop = new TPProto::SimpleEventLoop();

    ClientASL * listener = new ClientASL();

    layer = new TPProto::AdminLayer();
    layer->setClientString("tpadmin-cpp/0.0.1");
    layer->setLogger(logger);
    layer->setEventLoop(eventloop);
    layer->setAdminStatusListener(listener);

    commands.clear();

    commands.insert(new QuitCommand());
    commands.insert(new OpenCommand());
    commands.insert(new LoginCommand());
    commands.insert(new CloseCommand());

    halt = false;
}

Session::~Session()
{
    if(console != NULL)
        delete console;

    delete layer;
    delete logger;
    delete eventloop;
}

