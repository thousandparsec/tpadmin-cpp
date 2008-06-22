#include <string.h>

#include <tprl/console.h>
#include <tprl/rlcommand.h>

#include <tpproto/adminlayer.h>

#include "consolelogger.h"

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
        if(Session::getSession()->getAdminLayer()->getStatus() != TPProto::asDisconnected)
            Session::getSession()->getAdminLayer()->disconnect();
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
        Session::getSession()->getAdminLayer()->connect(cmdline);
    }
};

class LoginCommand : public tprl::RLCommand
{
  public:
    LoginCommand() : tprl::RLCommand()
    {
        name = "login";
        help = "Log in to administration on a TP server.";
    }

    void action(const std::string & cmdline)
    {
        size_t p = cmdline.find(' ');
        Session::getSession()->getAdminLayer()->login(cmdline.substr(0, p), cmdline.substr(p + 1));
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
        if(Session::getSession()->getAdminLayer()->getStatus() != TPProto::asDisconnected)
            Session::getSession()->getAdminLayer()->disconnect();
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

TPProto::AdminLayer * Session::getAdminLayer() const
{
    return layer;
}

tprl::Console * Session::getConsole() const
{
    return myConsole;
}

Session::Session()
{
    logger = new ConsoleLogger();

    layer = new TPProto::AdminLayer();
    layer->setClientString("tpadmin-cpp/0.0.1");
    layer->setLogger(logger);
    //layer->setEventLoop(eventloop);

    commands.clear();

    commands.insert(new QuitCommand());
    commands.insert(new OpenCommand());
    commands.insert(new LoginCommand());
    commands.insert(new CloseCommand());

    halt = false;
}

Session::~Session()
{
    if(myConsole != NULL)
        delete myConsole;
}

