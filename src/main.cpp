#include "logger/logger.h"
#include "machine/machine.h"
#include "display/display.h"



int main(int argc, char *args[])
{
	auto display = new Display();
	if (!display->Init())
	{
		return 1;
	}

	logger::PrintLine(logger::LogType::INFO, "Starting NES emulation");

	auto machine = new Machine();
	machine->Init();
	//machine->RunCPUTest(5000);
	machine->RunROM("roms/donkey_kong.nes", display);
	
	delete machine;
	delete display;
	return 0;
}