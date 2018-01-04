#include "debugcommand.hpp"

// ------------------------------------------------------------------------
DebugPrintCommand::DebugPrintCommand(const string &command) {
  nall::lstring parts;
  parts.split<2>(" ", command);

  if (!parts.size() == 2) {
    level = Fatal;
    message = "Not enough parameters for command";
  } else {
    if (parts[1] == "TRACE") { level = Trace; }
    else if (parts[1] == "DEBUG") { level = Debug; }
    else if (parts[1] == "INFO") { level = Info; }
    else if (parts[1] == "NOTICE") { level = Notice; }
    else if (parts[1] == "WARN") { level = Warn; }
    else if (parts[1] == "ERROR") { level = Error; }
    else if (parts[1] == "FATAL") { level = Fatal; }
    else {
      level = Info;
    }

    message = nall::substr(command, 7 + parts[1].length(), command.length() - 7 - parts[1].length());
  }
}

// ------------------------------------------------------------------------
void DebugPrintCommand::execute() {
  string color, prefix;

  switch (level) {
    case Trace: color = "888888"; prefix = "[TRACE]"; break;
    case Debug: color = "888888"; prefix = "[TRACE]"; break;
    default:
    case Info: color = "008800"; prefix = "[INFO]"; break;
    case Notice: color = "888800"; prefix = "[NOTICE]"; break;
    case Warn: color = "ff8800"; prefix = "[WARN]"; break;
    case Error: color = "ff0000"; prefix = "[ERROR]"; break;
    case Fatal: color = "ff0000"; prefix = "[FATAL]"; break;
  }

  debugger->echo(string() << "<font color='#" << color << "'> " << prefix << " " << message << "</font><br>");
}

// ------------------------------------------------------------------------
nall::string DebugPrintCommand::dump() const {
  string levelText = "INFO";

  switch (level) {
  case Trace: levelText = "TRACE"; break;
  case Debug: levelText = "DEBUG"; break;
  case Info: levelText = "INFO"; break;
  case Notice: levelText = "NOTICE"; break;
  case Warn: levelText = "WARN"; break;
  case Error: levelText = "ERROR"; break;
  case Fatal: levelText = "FATAL"; break;
  }

  return string("PRINT ", levelText, " ", message);
}
