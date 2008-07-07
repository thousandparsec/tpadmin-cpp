/*  ClientASL
 *
 *  Copyright (C) 2008  Lee Begg and the Thousand Parsec Project
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

#include "session.h"
#include "consolelogger.h"

#include "clientasl.h"

ClientASL::ClientASL()
{
}

ClientASL::~ClientASL()
{
}

void ClientASL::connected()
{
}

bool ClientASL::redirected(const std::string & url)
{
    return false;
}

void ClientASL::disconnected()
{
    // reset the local command set on disconnect
    Session::getSession()->resetCommands();
    Session::getSession()->getLogger()->warning("Disconnected from server.");
}

void ClientASL::loggedIn(bool state)
{
    // retrieve the list of commands on login
    if(state)
        Session::getSession()->getCommands();
}
