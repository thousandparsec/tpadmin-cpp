/*  ConsoleLogger class
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
#include <time.h>

#include <tprl/console.h>

#include "session.h"

#include "consolelogger.h"

ConsoleLogger::ConsoleLogger() : dispdebug(false)
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::error(const char * mesg, ...)
{
    char timeStr[30];
    time_t currTime = time(NULL);

    strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
    std::cout << "\r" << timeStr << "\e[31;1m < Error > \e[0m" << mesg << std::endl;

    tprl::Console::getConsole()->redrawLineForced();
}

void ConsoleLogger::warning(const char * mesg, ...)
{
    char timeStr[30];
    time_t currTime = time(NULL);

    strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
    std::cout << "\r" << timeStr << "\e[33;1m <Warning> \e[0m" << mesg << std::endl;

    tprl::Console::getConsole()->redrawLineForced();
}

void ConsoleLogger::info(const char * mesg, ...)
{
    char timeStr[30];
    time_t currTime = time(NULL);

    strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
    std::cout << "\r" << timeStr << "\e[32;1m < Info  > \e[0m" << mesg << std::endl;

    tprl::Console::getConsole()->redrawLineForced();
}

void ConsoleLogger::debug(const char * mesg, ...)
{
    if(dispdebug){
        char timeStr[30];
        time_t currTime = time(NULL);

        strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
        std::cout << "\r" << timeStr << " < Debug > " << mesg << std::endl;

        tprl::Console::getConsole()->redrawLineForced();
    }
}

void ConsoleLogger::toggleDebug()
{
    dispdebug = !dispdebug;
}
