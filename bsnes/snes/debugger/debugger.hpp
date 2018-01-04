class Debugger {
public:
  enum class BreakEvent : unsigned {
    None,
    BreakpointHit,
    CPUStep,
    SMPStep,
    SA1Step,
    SFXStep,
  } break_event;

  struct Trigger {
    Trigger() : addr(0), data(NULL) {}
    Trigger(unsigned addr, void *data) : addr(addr), data(data) {}
    unsigned addr;
    void *data;
  };
  linear_vector<Trigger> triggers;
  function<void (void*)> trigger_function;
  void add_trigger(unsigned addr, void *data);
  void remove_trigger(unsigned addr, void *data);

  enum { Breakpoints = 8,
         SoftBreakCPU = Breakpoints,
         SoftBreakSA1,
  };
  struct Breakpoint {
    bool enabled;
    unsigned addr;
    unsigned addr_end; // 0 = unused
    signed data;       //-1 = unused

    enum class Mode : unsigned { Exec = 1, Read = 2, Write = 4 };
    unsigned mode;

    enum class Source : unsigned { CPUBus, APURAM, VRAM, OAM, CGRAM, SA1Bus, SFXBus } source;
    unsigned counter;  //number of times breakpoint has been hit since being set
  } breakpoint[Breakpoints];
  unsigned breakpoint_hit;
  void trigger_test(unsigned addr);
  void breakpoint_test(Breakpoint::Source source, Breakpoint::Mode mode, unsigned addr, uint8 data);

  bool step_cpu;
  bool step_smp;
  bool step_sa1;
  bool step_sfx;
  bool bus_access;
  bool break_on_wdm;
  bool break_on_brk;
  bool enable_debug_interface;

  void writeDebugPort(uint32 addr, uint8 data);
  uint16 debugPort;
  function<void (uint16)> debug_port_event;

  struct DMALogCategories {
    enum {
      None     = 0,
      VRAM     = 1 << 0,
      OAM      = 1 << 1,
      CGRAM    = 1 << 2,
      Other    = 1 << 3
    };
  };
  bool log_dma;
  uint32 log_dma_flags;

  enum class StepType : unsigned {
    None, StepInto, StepOver, StepOut
  } step_type;
  int call_count;
  bool step_over_new;

  enum class MemorySource : unsigned { CPUBus, APUBus, APURAM, VRAM, OAM, CGRAM, CartROM, CartRAM, SA1Bus, SFXBus };
  uint8 read(MemorySource, unsigned addr);
  void write(MemorySource, unsigned addr, uint8 data);

  void log(const char *message, const char *color = NULL);
  void logv(const char *format, const char *color = NULL, ...);
  function<void (const char*, const char*)> logger;

  Debugger();
};

extern Debugger debugger;
