
void Application::parseArguments() {
  const QStringList argv = app->arguments();

  string fileName = "";
  bool processSwitches = true;

  for(unsigned i = 1; i < argv.size(); i++) {
    const string arg = argv.at(i).toUtf8().data();

    if(arg == "--") { processSwitches = false; continue; }

    if(arg[0] == '-' && processSwitches) {
      const string param = i + 1 < argv.size() ? argv.at(i + 1).toUtf8().data() : "";

      bool usesParam = parseArgumentSwitch(arg, param);
      if (usesParam) { i++; continue; }
    } else {
      if(fileName == "") fileName = arg;
    }
  }

  if(fileName != "") {
    cartridge.loadNormal(fileName);
  }
}

// returns true if argument uses parameter
bool Application::parseArgumentSwitch(const string& arg, const string& parameter) {
  if(arg == "--help" || arg == "-h") { printArguments(); return false; }

  #if defined(DEBUGGER)
  if(arg == "--show-debugger") { debugger->show(); return false; }

  if(arg == "--break-immediately") {
    application.debug = true;
    application.debugrun = false;
    debugger->synchronize();
    return false;
  }

  if(arg == "--break-on-wdm") {
    breakpointEditor->setBreakOnWDM(true);
    return false;
  }

  if(arg == "--break-on-brk") {
    breakpointEditor->setBreakOnBRK(true);
    return false;
  }

  if(arg == "--enable-debug-interface") {
    SNES::debugger.enable_debug_interface = true;
    return false;
  }

  if(arg == "--breakpoint" || arg == "-b") {
    if(parameter == "" || parameter[0] == '-') return false;

    breakpointEditor->addBreakpoint(parameter);

    return true;
  }

  #endif

  return false;
}

void Application::printArguments() {
  string filepath = app->applicationFilePath().toUtf8().data();

  puts(string("Usage: ", notdir(filepath), " [options] filename\n"));
  puts("  -h / --help                       show help");
  #if defined(DEBUGGER)
  puts("  --show-debugger                   open debugger window on startup\n"
       "  --break-immediately               break when loading the cartridge\n"
       "  --break-on-wdm                    break on WDM opcode\n"
       "  --break-on-brk                    break on BRK opcode\n"
       "  --enable-debug-interface          enables the debug port at $420E/$420F\n"
       "  -b / --breakpoint <breakpoint>    add breakpoint\n"
       "\n"
       "Breakpoint format: <addr>[-<addr end>][=<value>][:<rwx>[:<source>]]\n"
       "                   rwx = read / write / execute flags\n"
       "                   source = cpu, smp, vram, oam, cgram, sa1, sfx");
  #endif
}
