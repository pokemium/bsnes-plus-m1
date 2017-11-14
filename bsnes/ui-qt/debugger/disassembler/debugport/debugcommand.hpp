#ifndef __DEBUGCOMMAND__H__
#define __DEBUGCOMMAND__H__

// ------------------------------------------------------------------------
class DebugCommand {
public:
  DebugCommand() {}
  virtual ~DebugCommand() {}

  virtual void execute() = 0;

  virtual nall::string dump() const {
    return "";
  }
};

// ------------------------------------------------------------------------
class DebugPrintCommand : public DebugCommand {
public:
  enum Level { TRACE, DEBUG, INFO, NOTICE, WARN, ERROR, FATAL };

  DebugPrintCommand(const string &command);
  virtual void execute();
  virtual nall::string dump() const;

  Level level;
  string message;
};

// ------------------------------------------------------------------------

#endif
