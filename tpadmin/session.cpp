#include <string.h>

#include <tprl/console.h>
#include <tprl/rlcommand.h>

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
        //TODO - close server connection, if any
        Session::getSession()->stopMainLoop();
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
        //TODO - open new server connection
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
        //TODO - close server connection, if any
    }
};

Session * Session::myInstance = NULL;

Session * Session::getSession()
{
    if(myInstance == NULL)
        myInstance = new Session();
    return myInstance;
}

void Session::start()
{
    if(myConsole == NULL)
    {
        myConsole = tprl::Console::getConsole();
        myConsole->setCatchSignals(false);
        myConsole->setUseHistory(true);
        myConsole->setCommandSet(&commands);
        myConsole->setPrompt("tpadmin-cpp> ");
        myConsole->readLine_nb_start();
    }
}

void Session::stop()
{
    myConsole->readLine_nb_stop();
}

void Session::mainLoop()
{
    while(!halt)
    {
        myConsole->readLine_nb_inputReady();
    }
}

void Session::stopMainLoop()
{
    halt = true;
}

void Session::addCommand(tprl::RLCommand * command)
{
    commands.insert(command);
}

Session::Session()
{
    commands.clear();

    commands.insert(new QuitCommand());
    commands.insert(new OpenCommand());
    commands.insert(new CloseCommand());

    halt = false;
}

Session::~Session()
{
    if(myConsole != NULL)
        delete myConsole;
}

