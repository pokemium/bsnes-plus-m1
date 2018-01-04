#ifndef __DEBUGPORT_H
#define __DEBUGPORT_H

class DebugPort {
public:
  DebugPort();
  ~DebugPort();

  void allocate(uint32_t index);
  void addCommand(uint32_t index, const string &command);

  void execute(SNES::uint16 command);
  void setup();

  class DebugCommand **commands;
  uint32_t numAllocatedCommands;
  uint32_t numCommands;
};

extern DebugPort debugPort;

#endif
