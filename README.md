# bsnes-plus

This bnes-plus version is a development fork used to extend to original bnes-plus
with some new features, which are useful if you develop own games.

This fork is only tested under linux, yet.

For further details about bnes-plus, see the [original repository](https://github.com/devinacker/bsnes-plus).

## What's new

![New debugger](./debugger.jpg?raw=true "New debugger")

- Merged disassembler and debugger into one window
- The disassembler has some simple syntax highlighting
- The disassembler supports the WLA symbol files (also some additional own features)
- Break on BRK
- Additional start up parameters
- A custom debug port to allow some debug stuff while developing own games (see below)
- Toggle breakpoints in disassembler by clicking on the address
- Symbol index with filter and direct option to toggle breakpoints
- Add comments to code from disassembler (to remember some code)


## New command line arguments

- `--enable-debug-interface` Enables the debug interface (see below)
- `--break-immediately` Stops the emulator immediately (debug from the beginning)
- `--break-on-brk` Stops the emulator on BRK opcodes


## The symbol file format

It is possible to place a symbol file beside the ROM to import symbols.

The format is an extended version of the WLA symbol files

    ; myrom.sym

    [labels]
    00:8000 start
    00:8012 my_own_routine


## The command interface

When enabled (using the command line), it is possible to execute debug commands
using the program code.

Simply write the command number to the registers `$420E` and `$420F` and define
the actual command in the symbol file.

Example for assembler:

    LDX   #$1 ; the command ID
    STX   $420E

Example within the symbol file

    ; myrom.sym

    [debug]
    0001 PRINT INFO Hello world!

The first 4 digits are hex digits defining the command ID (start with low IDs).

This example would write `Hello world!` to the debuggers console.

### Implemented commands

| Command | Usage |
| ------- | ----- |
| PRINT | `LEVEL MESSAGE...` - Level might be DEBUG, TRACE, INFO, NOTICE, WARN, ERROR or FATAL. Prints a debug message to the console |


## Coming soon

- Add symbols from disassembler (when reverse engineering)
