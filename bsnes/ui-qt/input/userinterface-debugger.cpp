InputGroup userInterfaceDebugger(InputCategory::UserInterface, "Debugger");

namespace UserInterfaceDebugger {

struct DebuggerClear : HotkeyInput {
  void pressed() {
    debugger->clear();
  }

  DebuggerClear() : HotkeyInput("Clear", "input.userInterface.debugger.clear") {
    name = "Control+KB0::K";
    userInterfaceDebugger.attach(this);
  }
} debuggerClear;

}
