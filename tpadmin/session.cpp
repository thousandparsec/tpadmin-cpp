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

#include <string.h>

#include <tprl/console.h>
#include <tprl/rlcommand.h>

#include <tpproto/adminlayer.h>

#include "consolelogger.h"

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
        Session::getSession()->getAdminLayer()->connect(cmdline);
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
        size_t p = cmdline.find(' ');
        Session::getSession()->getAdminLayer()->login(cmdline.substr(0, p), cmdline.substr(p + 1));
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
    if(myConsole == NULL)
    {
        myConsole = tprl::Console::getConsole();
        myConsole->setCatchSignals(false);
        myConsole->setUseHistory(true);
        myConsole->setCommandSet(&commands);
        myConsole->setPrompt("tpadmin-cpp> ");
        myConsole->readLine_nb_start();
    }
}

void Session::stop()
{
    myConsole->readLine_nb_stop();
}

void Session::mainLoop()
{
    while(!halt)
    {
        myConsole->readLine_nb_inputReady();
    }
}

void Session::stopMainLoop()
{
    halt = true;
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
    return myConsole;
}

Session::Session()
{
    logger = new ConsoleLogger();

    layer = new TPProto::AdminLayer();
    layer->setClientString("tpadmin-cpp/0.0.1");
    layer->setLogger(logger);
    //layer->setEventLoop(eventloop);

    commands.clear();

    commands.insert(new QuitCommand());
    commands.insert(new OpenCommand());
    commands.insert(new LoginCommand());
    commands.insert(new CloseCommand());

    halt = false;
}

Session::~Session()
{
    if(myConsole != NULL)
        delete myConsole;
}

