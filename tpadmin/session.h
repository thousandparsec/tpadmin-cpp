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

#ifndef SESSION_H
#define SESSION_H

#include <set>
#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace tprl
{
    class RLCommand;
}

namespace TPProto
{
    class AdminLayer;
    class SimpleEventLoop;
    class CommandDescription;
}

class Console;
class ConsoleLogger;

class Session
{
  public:
    static Session * getSession();

    void start();
    void stop();

    void getCommands();
    void receiveCommands(std::set<uint32_t> ids);
    void receiveCommandDesc(boost::shared_ptr<TPProto::CommandDescription> cd);

    void addCommand(tprl::RLCommand * command);
    void resetCommands();

    TPProto::AdminLayer * getAdminLayer() const;
    Console * getConsole() const;
    ConsoleLogger * getLogger() const;

  private:
    Session();
    virtual ~Session();

    static Session * myInstance;

    Console * console;
    std::set<tprl::RLCommand*> commands;
    unsigned int local;

    ConsoleLogger * logger;
    TPProto::SimpleEventLoop * eventloop;
    TPProto::AdminLayer * layer;

};

#endif
