# bsnes-plus

bsnes-plus (or bsnes+) is a fork of bsnes (based on bsnes-classic) intended to
introduce some new features and improvements, mostly aimed at debugging.

## What's new

- "Step over" and "step out" buttons in debugger
- Improved debugger UI with register editing
- Redesigned memory editor and breakpoint editor
- Improved handling of address mirroring for breakpoints (extends to the entire address space, not just RAM)
- Real-time code and data highlighting in memory editor, with fast searching for known code/data locations and unexplored regions
- Cartridge ROM and RAM views in memory editor for mapper-agnostic analysis
- Enhanced VRAM, sprite, and tilemap viewing
- SA-1 disassembly and debugging
- SA-1 bus and BW-RAM viewing and (partial) usage logging
- Super FX disassembly and debugging
- Super FX bus viewing and usage logging

Non-debugging features:

- Satellaview / BS-X support
- SPC file dumping
- SPC output visualizer (keyboards & peak meters)
- IPS and BPS soft patching
- Multiple emulation improvements backported from bsnes/higan (mostly via bsnes-classic)

## Binaries

- Windows binaries are available in devinacker's [upstream repository](https://github.com/devinacker/bsnes-plus/releases)
- macOS binaries are available [here](https://goo.gl/06Ofmd)

## Building on Windows

- Get [mingw-w64](http://mingw-w64.yaxm.org/doku.php/download)
- Install [Qt 4.8.6](http://download.qt.io/archive/qt/) and make sure its `bin` directory is in your path
- Run `mingw32-make`

## Building on macOS

- Install a C++ toolchain ([Xcode](https://developer.apple.com) is probably the easiest route)
- Install [Qt](https://www.qt.io/download-open-source/) (required components: `macOS` and `Qt WebEngine`)
- Add a `qtpath` environment variable pointing to your Qt installation (ie. add `export qtpath=$HOME/Qt/5.9.1/clang_64` to .bash_profile)
- Run `make` from repository root directory
- Alternatively run `make dist` to create stand-alone app bundles and a redistributable archive in the `distribution` directory


## Building on Linux / other *nix

As there is no ``configure`` step, make sure necessary Qt4/X11 packages are installed. On a Debian/Ubuntu system, it would require a command like:

```
apt-get install libqt4-dev libqt4-dev-bin libxv-dev libsdl1.2-dev libao-dev
libopenal-dev g++
```

Afterwards, run ``make`` and if everything works out correctly you will find the output binary in the ``out/`` directory.

The snesfilter, snesreader, and supergameboy plugins can all be built by running make (or mingw32-make) after you've configured your environment to build bsnes itself.
After building, just copy the .dll, .so, or .dylib files into the same directory as bsnes itself.

This fork of bsnes doesn't include the alternate UI based on byuu's `phoenix` library. The purpose of this fork is primarily to add additional UI functionality and I have no intention of implementing every new feature twice using completely different libraries just to keep both versions of the UI at parity.

bsnes v073 and its derivatives are licensed under the GPL v2; see *Help > License ...* for more information.

## Contributors

See *Help > Documentation ...* for a list of authors.
