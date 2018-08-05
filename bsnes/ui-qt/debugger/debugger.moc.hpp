class Debugger : public Window {
  Q_OBJECT

public:
  QMenuBar *menu;
  QMenu *menu_tools;
  QAction *menu_tools_breakpoint;
  QAction *menu_tools_memory;
  QAction *menu_tools_propertiesViewer;
  QAction *menu_tools_measurements;
  QMenu *menu_ppu;
  QAction *menu_ppu_tileViewer;
  QAction *menu_ppu_tilemapViewer;
  QAction *menu_ppu_oamViewer;
  QAction *menu_ppu_cgramViewer;
  QMenu *menu_misc;
  QAction *menu_misc_clear;
  QAction *menu_misc_cacheUsage;
  QAction *menu_misc_saveBreakpoints;
  QAction *menu_misc_showHClocks;

  QVBoxLayout *layout;
  QSplitter *mainLayout;
  LogView *console;
  QToolBar *toolBar;
  QToolButton *runBreak;
  QToolButton *stepInstruction;
  QToolButton *stepOver;
  QToolButton *stepOut;
  QToolButton *traceMask;

  QCheckBox *logDMA;
  QGroupBox *logDMA_group;
  QCheckBox *logDMA_vram;
  QCheckBox *logDMA_oam;
  QCheckBox *logDMA_cgram;
  QCheckBox *logDMA_other;
  QPushButton *logClearButton;

  class DebuggerView *debugCPU;
  class DebuggerView *debugSFX;
  class DebuggerView *debugSA1;
  class DebuggerView *debugSMP;

  class SymbolMap *symbolsCPU;
  class SymbolMap *symbolsSA1;
  class SymbolMap *symbolsSMP;

  void modifySystemState(unsigned);
  void echo(const char *message, const char *color = "");
  void event();
  void autoUpdate();

  Debugger();

  enum MenuAction {
    MainWindow = 0,
    DisassemblerWindow,
    BreakpointsWindow,
    MemoryWindow,
    PropertiesWindow,
    MeasurementsWindow,
    TileWindow,
    TilemapWindow,
    OAMWindow,
    CGRAMWindow,
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
  void createMemoryEditor();
  void setLogDMAState(int);

private:
  inline void switchWindow();

  unsigned frameCounter;
};

extern Debugger *debugger;

