#include "logger/logger.h"
#include "machine/machine.h"



int main()
{
	logger::PrintLine(logger::LogType::INFO, "Starting NES emulation");

	auto *machine = new Machine();
	machine->Init();
	machine->RunCPUTest(5000);
	//machine->RunROM("roms/donkey_kong.nes");
	
	delete machine;
	return 0;
}