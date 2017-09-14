#include <list>

struct LogMessage {
  LogMessage(){}
  LogMessage(const string& plain_message, const string& color = "") {
    message = string() << plain_message;
    message.replace("\n", "<br>");
    message.replace(" ", "&nbsp;");
    if (color.length()) {
      message = string() << "<font color='" << color << "'>" << message << "</font>";
    }
  }

  string message;
};

struct HtmlLogMessage : LogMessage {
  HtmlLogMessage(const string& html_message) {
    message = html_message;
  }
};

class Debugger : public Window {
  Q_OBJECT

public:
  QMenuBar *menu;
  QMenu *menu_tools;
  QAction *menu_tools_disassembler;
  QAction *menu_tools_breakpoint;
  QAction *menu_tools_memory;
  QAction *menu_tools_propertiesViewer;
  QMenu *menu_ppu;
  QAction *menu_ppu_vramViewer;
  QAction *menu_ppu_tilemapViewer;
  QAction *menu_ppu_oamViewer;
  QAction *menu_ppu_cgramViewer;
  QMenu *menu_misc;
  QAction *menu_misc_clear;
  QAction *menu_misc_options;

  QHBoxLayout *layout;
  QPlainTextEdit *console;
  QVBoxLayout *consoleLayout;
  QVBoxLayout *controlLayout;
  QHBoxLayout *commandLayout;
  QToolButton *runBreak;
  QToolButton *stepInstruction;
  QToolButton *stepOver;
  QToolButton *stepOut;
  QCheckBox *stepCPU;
  QCheckBox *stepSMP;
  QCheckBox *stepSA1;
  QCheckBox *stepSFX;
  QCheckBox *traceCPU;
  QCheckBox *traceSMP;
  QCheckBox *traceSA1;
  QCheckBox *traceSFX;
  QCheckBox *traceMask;

  QCheckBox *logDMA;
  QGroupBox *logDMA_group;
  QCheckBox *logDMA_vram;
  QCheckBox *logDMA_oam;
  QCheckBox *logDMA_cgram;
  QCheckBox *logDMA_other;

  QWidget *spacer;

  void paintEvent(QPaintEvent*);

  void modifySystemState(unsigned);
  void updateConsole();
  void echo(const char *html_message);
  void log(const char *plain_message, const char *color);
  void log(const LogMessage message);
  void event();
  void autoUpdate();

  Debugger();

  enum MenuAction {
    MainWindow = 0,
    DisassemblerWindow,
    BreakpointsWindow,
    MemoryWindow,
    PropertiesWindow,
    VRAMWindow,
    TilemapWindow,
    OAMWindow,
    CGRAMWindow,
    OptionsWindow,
    ClearConsole
  } MenuAction_t;

  void menuAction(MenuAction);

public slots:
  void clear();
  void synchronize();
  void frameTick();

  void toggleRunStatus();
  void stepAction();
  void stepOverAction();
  void stepOutAction();

  void setLogDMAState(int);

private:
  inline void switchWindow();

  std::list<LogMessage> messageBuffer;
  unsigned frameCounter;
};

extern Debugger *debugger;
