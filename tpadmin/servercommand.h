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

#ifndef SERVERCOMMAND_H
#define SERVERCOMMAND_H

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

#include <tprl/rlcommand.h>

namespace TPProto
{
    class CommandDescription;
    class CommandParameter;
}

class ServerCommand : public tprl::RLCommand{
  public:
    ServerCommand();
    ServerCommand(boost::shared_ptr<TPProto::CommandDescription> cd);
    virtual ~ServerCommand();

    void action(const std::string & cmdline);

  private:
    int ctype;
    std::list<TPProto::CommandParameter*> params;

};

#endif
