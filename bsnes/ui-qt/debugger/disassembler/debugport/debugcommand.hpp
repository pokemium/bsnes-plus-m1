#ifndef __DEBUGCOMMAND__H__
#define __DEBUGCOMMAND__H__

// ------------------------------------------------------------------------
class DebugCommand {
public:
  virtual void execute() = 0;
};

// ------------------------------------------------------------------------
class DebugPrintCommand : public DebugCommand {
public:
  enum Level { TRACE, DEBUG, INFO, NOTICE, WARN, ERROR, FATAL };

  DebugPrintCommand(const string &command);
  virtual void execute();

  Level level;
  string message;
};

// ------------------------------------------------------------------------

#endif
