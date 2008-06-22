#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include <tpproto/logger.h>

class ConsoleLogger : public TPProto::Logger{
  public:
    ConsoleLogger();
    virtual ~ConsoleLogger();

    void error(const char * mesg, ...);
    void warning(const char * mesg, ...);
    void info(const char * mesg, ...);
    void debug(const char * mesg, ...);
};

#endif
