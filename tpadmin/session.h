#ifndef SESSION_H
#define SESSION_H

#include <set>

namespace tprl
{
    class Console;
    class RLCommand;
}

namespace TPProto
{
    class AdminLayer;
}

class Session
{
  public:
    static Session * getSession();

    void start();
    void stop();

    void mainLoop();
    void stopMainLoop();

    void addCommand(tprl::RLCommand * command);

  private:
    Session();
    virtual ~Session();

    static Session * myInstance;

    tprl::Console * myConsole;
    std::set<tprl::RLCommand*> commands;

    TPProto::AdminLayer * layer;

    volatile bool halt;

};

#endif
