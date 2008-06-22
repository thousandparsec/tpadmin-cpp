#include <iostream>
#include <time.h>

#include "consolelogger.h"

ConsoleLogger::ConsoleLogger()
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::error(char * mesg, ...)
{
    char timeStr[30];
    time_t currTime = time(NULL);

    strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
    std::cout << "\r" << timeStr << "\e[31;1m < Error > " << mesg << std::endl;

    Session::getSession()->myConsole->redrawLineForced();
}

void ConsoleLogger::warning(char * mesg, ...)
{
    char timeStr[30];
    time_t currTime = time(NULL);

    strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
    std::cout << "\r" << timeStr << "\e[33;1m <Warning> " << mesg << std::endl;

    Session::getSession()->myConsole->redrawLineForced();
}

void ConsoleLogger::info(char * mesg, ...)
{
    char timeStr[30];
    time_t currTime = time(NULL);

    strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
    std::cout << "\r" << timeStr << "\e[32;1m < Info  > " << mesg << std::endl;

    Session::getSession()->myConsole->redrawLineForced();
}

void ConsoleLogger::debug(char * mesg, ...)
{
    char timeStr[30];
    time_t currTime = time(NULL);

    strftime(timeStr, 30, "%F %H:%M:%S", localtime(&currTime));
    std::cout << "\r" << timeStr << " < Debug > " << mesg << std::endl;

    Session::getSession()->myConsole->redrawLineForced();
}
