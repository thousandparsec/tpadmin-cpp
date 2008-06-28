/*  Console - libtprl Console wrapper socket for use in an EventLoop
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

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <tprl/console.h>

#include <tpproto/connection.h>

#include "console.h"

class ConsoleConnection : public TPProto::Connection{
  public:
    ConsoleConnection(){}
    ~ConsoleConnection(){}

    void readyToRead()
    {
        tprl::Console::getConsole()->readLine_nb_inputReady();
    }

    void readyToSend(){}
};

Console::Console() : TPSocket(), status(0)
{
    rlconsole = tprl::Console::getConsole();
    rlconsole->setCatchSignals(false);
    rlconsole->setUseHistory(true);
    rlconsole->setPrompt("tpadmin-cpp> ");

    setConnection(new ConsoleConnection());
}

Console::~Console()
{
    delete rlconsole;
}

bool Console::isConnected()
{
    return status == 1;
}

bool Console::connect()
{
    if(status == 0){
        fd = STDIN_FILENO;
        status = 1;
        return true;
    }
    return false;
}

void Console::disconnect()
{
    status = 0;
}

int Console::send(const char * data, int len)
{
    return 0;
}

int Console::recv(int len, char * data)
{
    return 0;
}

void Console::start()
{
    rlconsole->readLine_nb_start();
}

void Console::stop()
{
    rlconsole->readLine_nb_stop();
}

void Console::setCommandSet(std::set<tprl::RLCommand*> * set)
{
    rlconsole->setCommandSet(set);
}
