#include "logger/logger.h"
#include "machine/machine.h"
#include "machine/ppu/display/display.h"



int main(int argc, char *args[])
{
	logger::PrintLine(logger::LogType::INFO, "Starting NES emulation");

	auto machine = new Machine();
	machine->Init();
	machine->ppu.display.Init();
	//machine->RunCPUTest(5000);
	machine->RunROM("roms/donkey_kong.nes");
	
	delete machine;
	return 0;
}