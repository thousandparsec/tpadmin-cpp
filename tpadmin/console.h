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

#ifndef CONSOLE_H
#define CONSOLE_H

#include <set>

#include <tpproto/tpsocket.h>

namespace tprl
{
    class Console;
    class RLCommand;
}

class Console : public TPProto::TPSocket{
  public:
    Console();
    virtual ~Console();

    bool isConnected();
    bool connect();
    void disconnect();
    int send(const char * data, int len);
    int recv(int len, char * data);

    void start();
    void stop();

    void setCommandSet(std::set<tprl::RLCommand*> * set);

  private:
    tprl::Console * rlconsole;

    int status;
};

#endif
