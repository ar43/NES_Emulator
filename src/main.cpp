#include "logger/logger.h"
#include "machine/machine.h"



int main()
{
	logger::PrintLine(logger::LogType::INFO, "Starting NES emulation");

	auto *machine = new Machine();
	
	machine->cpu.DumpRegisters();

	return 0;
}