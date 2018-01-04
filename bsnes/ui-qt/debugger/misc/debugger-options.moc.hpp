class DebuggerOptions : public Window {
  Q_OBJECT

public:
  QVBoxLayout *layout;
  QCheckBox *cacheUsageBox;
  QCheckBox *saveBreakpointsBox;
  QCheckBox *saveSymbolsBox;
  QCheckBox *showHClocksBox;

  void synchronize();
  DebuggerOptions();

public slots:
  void toggleCacheUsage(bool);
  void toggleSaveBreakpoints(bool);
  void toggleSaveSymbols(bool);
  void toggleHClocks(bool);
};

extern DebuggerOptions *debuggerOptions;
