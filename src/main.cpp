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
	if(argc == 2)
		machine->RunROM(std::string(args[1]));
	
	delete machine;
	return 0;
}