# NES_Emulator

The emulator only works on Windows operating systems.

## How to compile

This project is compiled using Visual Studio 2019. Simply launch the solution file with Visual Studio and the project should compile out of the box.

The project uses:
* ISO C++ 17 standard
* SDL library (SDL, SDL_ttf, SDL_image) - for rendering, input, audio
* Windows API - for timing, menu bar, .ini file parsing

## How to use the emulator

The already compiled version is available under [Releases](https://github.com/ar43/NES_Emulator/releases).

To start the program, run NES_Emulator.exe. To run a rom, click on File->Open. Some roms are available with the release, located in the ./roms folder. Controls are seen under Options->Controls and can be freely configured (any changes are saved). Rom can be paused/reset/turned off under Game->... To access the debugger, simply click on Tools->Debug. The debugger only works on mappers that do not use PRG bankswitching. To see various hotkeys, click on Help->About. To reset all controls/settings, delete the ./data/config.ini file (the file gets created after first usage).