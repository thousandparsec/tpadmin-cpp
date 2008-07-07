/*  ServerCommand
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

#include <tpproto/adminlayer.h>
#include <tpproto/commanddesc.h>
#include <tpproto/commandparameter.h>

#include "session.h"
#include "consolelogger.h"

#include "servercommand.h"

ServerCommand::ServerCommand() : tprl::RLCommand()
{
}

ServerCommand::~ServerCommand()
{
}

void ServerCommand::action(const std::string & cmdline)
{
    // verify that the session is logged in
    if(Session::getSession()->getAdminLayer()->getStatus() != TPProto::asLoggedIn){
        Session::getSession()->getLogger()->error("Not logged in to server.");
        return;
    }

    unsigned int n = 0;
    size_t pprev = cmdline.find_first_not_of(' ', 0);
    size_t pcurr = cmdline.find_first_of(' ', pprev);

    // repopulate the parameter list from the description
    params = desc->getParameters();

    // parse the command line into the command parameters
    for(std::list<TPProto::CommandParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
        if(pcurr == std::string::npos && pprev == std::string::npos)
            break;
        if(!(*itcurr)->setValueFromString(cmdline.substr(pprev, pcurr - pprev)))
            break;
        pprev = cmdline.find_first_not_of(' ', pcurr);
        pcurr = cmdline.find_first_of(' ', pprev);
        n++;
    }

    if(n == params.size()){
        // send a command frame
        Session::getSession()->getAdminLayer()->sendCommand(desc, params);
    }else{
        Session::getSession()->getLogger()->error("Invalid command parameters.");
    }
}

void ServerCommand::setCommandType(boost::shared_ptr<TPProto::CommandDescription> cd)
{
    name = cd->getName();
    help = cd->getDescription();

    desc = cd;
    params = cd->getParameters();
}

