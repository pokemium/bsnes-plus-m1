#include "debugcommand.hpp"

// ------------------------------------------------------------------------
DebugPrintCommand::DebugPrintCommand(const string &command) {
  nall::lstring parts;
  parts.split<2>(" ", command);

  if (!parts.size() == 2) {
    level = FATAL;
    message = "Not enough parameters for command";
  } else {
    //enum Level { TRACE, DEBUG, INFO, NOTICE, WARN, ERROR, FATAL };

    if (parts[1] == "TRACE") { level = TRACE; }
    else if (parts[1] == "DEBUG") { level = DEBUG; }
    else if (parts[1] == "INFO") { level = INFO; }
    else if (parts[1] == "NOTICE") { level = NOTICE; }
    else if (parts[1] == "WARN") { level = WARN; }
    else if (parts[1] == "ERROR") { level = ERROR; }
    else if (parts[1] == "FATAL") { level = FATAL; }
    else {
      level = INFO;
    }

    message = nall::substr(command, 7 + parts[1].length(), command.length() - 7 - parts[1].length());
  }
}

// ------------------------------------------------------------------------
void DebugPrintCommand::execute() {
  string color, prefix;

  switch (level) {
    case TRACE: color = "888888"; prefix = "[TRACE]"; break;
    case DEBUG: color = "888888"; prefix = "[TRACE]"; break;
    default:
    case INFO: color = "008800"; prefix = "[INFO]"; break;
    case NOTICE: color = "888800"; prefix = "[NOTICE]"; break;
    case WARN: color = "ff8800"; prefix = "[WARN]"; break;
    case ERROR: color = "ff0000"; prefix = "[ERROR]"; break;
    case FATAL: color = "ff0000"; prefix = "[FATAL]"; break;
  }

  debugger->echo(string() << "<font color='#" << color << "'> " << prefix << " " << message << "</font><br>");
}

// ------------------------------------------------------------------------
nall::string DebugPrintCommand::dump() const {
  string levelText = "INFO";

  switch (level) {
  case TRACE: levelText = "TRACE"; break;
  case DEBUG: levelText = "DEBUG"; break;
  case INFO: levelText = "INFO"; break;
  case NOTICE: levelText = "NOTICE"; break;
  case WARN: levelText = "WARN"; break;
  case ERROR: levelText = "ERROR"; break;
  case FATAL: levelText = "FATAL"; break;
  }

  return string("PRINT ", levelText, " ", message);
}
