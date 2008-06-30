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
#include <string>
#include <boost/bind.hpp>

#include <tprl/rlcommand.h>

#include <tpproto/adminlayer.h>
#include <tpproto/simpleeventloop.h>
#include <tpproto/commanddesccache.h>

#include "console.h"
#include "consolelogger.h"
#include "clientasl.h"
#include "servercommand.h"

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
        Session::getSession()->stop();
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
    console = new Console();
    console->setCommandSet(&commands);
    if(console->connect())
        logger->debug("Console connected");
    console->start();

    eventloop->listenForSocketRead(console);
    eventloop->runEventLoop();
}

void Session::stop()
{
    console->disconnect();
    console->stop();

    if(layer->getStatus() != TPProto::asDisconnected)
        layer->disconnect();

    eventloop->endEventLoop();
}

void Session::getCommands()
{
    layer->getCommandDescCache()->requestCommandTypes(boost::bind(&Session::receiveCommands, this, _1));
    logger->info("Requested command list from server.");
}

void Session::receiveCommands(std::set<uint32_t> ids)
{
    for(std::set<uint32_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
        layer->getCommandDescCache()->requestCommandDescription((*itcurr), boost::bind(&Session::receiveCommandDesc, this, _1));
    }
}

void Session::receiveCommandDesc(boost::shared_ptr<TPProto::CommandDescription> cd)
{
    ServerCommand * ncmd = new ServerCommand();
    ncmd->setCommandType(cd);
    addCommand(ncmd);
}

void Session::addCommand(tprl::RLCommand * command)
{
    commands.insert(command);
}

TPProto::AdminLayer * Session::getAdminLayer() const
{
    return layer;
}

Console * Session::getConsole() const
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

    layer = new TPProto::AdminLayer();
    layer->setClientString("tpadmin-cpp/0.0.1");
    layer->setLogger(logger);
    layer->setEventLoop(eventloop);
    layer->setAdminStatusListener(new ClientASL());

    commands.clear();

    addCommand(new QuitCommand());
    addCommand(new OpenCommand());
    addCommand(new LoginCommand());
    addCommand(new CloseCommand());
}

Session::~Session()
{
    if(console != NULL)
        delete console;

    delete layer;
    delete logger;
    delete eventloop;
}

