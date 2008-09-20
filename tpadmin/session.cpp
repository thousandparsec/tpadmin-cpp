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
#include <tprl/commandalias.h>

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
        if(Session::getSession()->getAdminLayer()->getStatus() != TPProto::asDisconnected){
            Session::getSession()->getAdminLayer()->disconnect();
        }
    }
};

class DebugCommand : public tprl::RLCommand
{
  public:
    DebugCommand() : tprl::RLCommand()
    {
        name = "debug";
        help = "Toggles whether to display debug-level log messages.";
    }

    void action(const std::string & cmdline)
    {
        Session::getSession()->getLogger()->toggleDebug();
    }
};

Session * Session::myInstance = NULL;

/* getSession
 * Returns a reference to this unique instance of Session.
 */
Session * Session::getSession()
{
    if(myInstance == NULL)
        myInstance = new Session();
    return myInstance;
}

/* start
 * Starts the session.
 */
void Session::start()
{
    // prepare the console
    console = new Console();
    console->setCommandSet(&commands);
    if(console->connect())
        logger->debug("Console connected");
    console->start();

    // add the console to the event loop and start it
    eventloop->listenForSocketRead(console);
    eventloop->runEventLoop();
}

/* stop
 * Stops the session.
 */
void Session::stop()
{
    // stop the console
    console->disconnect();
    console->stop();

    // disconnect from the server
    if(layer->getStatus() != TPProto::asDisconnected)
        layer->disconnect();

    // stop the event loop
    eventloop->endEventLoop();
}

/* getCommands
 * Gets a list of command IDs from the server.
 */
void Session::getCommands()
{
    layer->getCommandDescCache()->requestCommandTypes(boost::bind(&Session::receiveCommands, this, _1));
    logger->info("Requested command list from server.");
}

/* receiveCommands
 * Callback for the list of command IDs.
 * Requests descriptions for each command type.
 * ids - The list of command type IDs.
 */
void Session::receiveCommands(std::set<uint32_t> ids)
{
    for(std::set<uint32_t>::iterator itcurr = ids.begin(); itcurr != ids.end(); ++itcurr){
        layer->getCommandDescCache()->requestCommandDescription((*itcurr), boost::bind(&Session::receiveCommandDesc, this, _1));
    }
}

/* receiveCommandDesc
 * Callback for comamnd descriptions.
 * Creates new ServerCommands for each command and adds them to the local set.
 * cd - The command description.
 */
void Session::receiveCommandDesc(boost::shared_ptr<TPProto::CommandDescription> cd)
{
    ServerCommand * ncmd = new ServerCommand();
    ncmd->setCommandType(cd);
    addCommand(ncmd);
}

/* addCommand
 * Adds a command to the local set.
 * command - The (tprl) command to add.
 */
void Session::addCommand(tprl::RLCommand * command)
{
    commands.insert(command);
}

/* resetCommands
 * Clears all non-builtin commands from the local set.
 */
void Session::resetCommands()
{
    std::set<tprl::RLCommand*>::iterator eol = commands.begin();
    for(unsigned int i = 0; i < local; i++)
        eol++;
    commands.erase(eol, commands.end());
}

/* getAdminLayer
 * Returns a reference to this session's AdminLayer.
 */
TPProto::AdminLayer * Session::getAdminLayer() const
{
    return layer;
}

/* getConsole
 * Returns a reference to this session's console.
 */
Console * Session::getConsole() const
{
    return console;
}

/* getLogger
 * Returns a reference to this session's logger.
 */
ConsoleLogger * Session::getLogger() const
{
    return logger;
}

/* Default constructor
 * Instantiates AdminLayer, event loop, and logger.
 * Adds builtin commands to the local set.
 */
Session::Session()
{
    logger = new ConsoleLogger();

    eventloop = new TPProto::SimpleEventLoop();

    // configure AdminLayer
    layer = new TPProto::AdminLayer();
    layer->setClientString("tpadmin-cpp/0.0.1");
    layer->setLogger(logger);
    layer->setEventLoop(eventloop);
    layer->setAdminStatusListener(new ClientASL());

    commands.clear();
    tprl::RLCommand* quit = new QuitCommand();
    addCommand(quit);
    addCommand(new OpenCommand());
    addCommand(new LoginCommand());
    addCommand(new CloseCommand());
    addCommand(new DebugCommand());
    tprl::CommandAlias* alias = new tprl::CommandAlias("exit");
    alias->setTarget(quit);
    addCommand(alias);

    local = commands.size();
}

/* Destructor
 * Deletes console, AdminLayer, event loop, and logger.
 */
Session::~Session()
{
    if(console != NULL)
        delete console;

    delete layer;
    delete logger;
    delete eventloop;
}

