#include "debugport.hpp"
#include "debugcommand.cpp"

DebugPort debugPort;

// ------------------------------------------------------------------------
DebugPort::DebugPort() {
  commands = new DebugCommand*[1];
  numAllocatedCommands = 1;
  numCommands = 0;
  commands[0] = NULL;
}

// ------------------------------------------------------------------------
DebugPort::~DebugPort() {
  for (uint32_t i=0; i<numAllocatedCommands; i++) {
    DebugCommand *cmd = commands[i];
    if (cmd) {
      delete cmd;
    }
  }
  delete[] commands;
}

// ------------------------------------------------------------------------
void DebugPort::setup() {
  SNES::debugger.debug_port_event = { &DebugPort::execute, this };
}

// ------------------------------------------------------------------------
void DebugPort::allocate(uint32_t index) {
  if (index >= numCommands) {
    numCommands = index + 1;
  }

  if (index < numAllocatedCommands) {
    return;
  }

  uint32_t newCommands = (index + 127) & ~127;
  commands = (DebugCommand**)realloc(commands, newCommands * sizeof(DebugCommand*));
  for (uint32_t i=numAllocatedCommands; i<newCommands; i++) {
    commands[i] = NULL;
  }

  numAllocatedCommands = newCommands;
}

// ------------------------------------------------------------------------
void DebugPort::execute(SNES::uint16 command) {
  if (command >= numCommands) {
    return;
  }

  commands[command]->execute();
}

// ------------------------------------------------------------------------
void DebugPort::addCommand(uint32_t index, const string &command) {
  nall::lstring parts;
  parts.split<1>(" ", command);
  allocate(index);

  if (parts[0] == "PRINT") {
    commands[index] = new DebugPrintCommand(command);
  } else {
    puts(string("Notice: Debug command ", parts[0], " is not supported."));
  }
}

// ------------------------------------------------------------------------
