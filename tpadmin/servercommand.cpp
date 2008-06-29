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

#include <tpproto/commanddesc.h>
#include <tpproto/commandparameter.h>

#include "servercommand.h"

ServerCommand::ServerCommand() : tprl::RLCommand()
{
}

ServerCommand::ServerCommand(boost::shared_ptr<TPProto::CommandDescription> cd) : tprl::RLCommand()
{
    name = cd->getName();
    help = cd->getDescription();
    ctype = cd->getCommandType();
    params = cd->getParameters();
}

ServerCommand::~ServerCommand()
{
}

void ServerCommand::action(const std::string & cmdline)
{
    size_t pcurr = 0, pprev = -1;
    bool done = false;
    unsigned int n = 0;

    // fill parameters
    for(std::list<TPProto::CommandParameter*>::iterator itcurr = params.begin(); itcurr != params.end(); ++itcurr){
        pcurr = cmdline.find(' ', pcurr);
        if(pcurr == std::string::npos)
            done = true;

        if(!(*itcurr)->setValueFromString(cmdline.substr(pprev + 1, pcurr)))
            break;
        pprev = pcurr;

        n++;
        if(done)
            break;
    }

    if(n == params.size()){
        // pack and send a command frame
    }else{
        // print usage
    }
}
