#include "logger/logger.h"
#include "machine/machine.h"



int main()
{
	logger::PrintLine(logger::LogType::INFO, "Starting NES emulation");

	auto *machine = new Machine();
	if (machine->LoadNES("cpu_testing/nestest.nes"))
	{
		if (machine->memory.LoadNES(machine->nes_data.get()))
		{
			machine->cpu.RunTest(&machine->memory, 0, 5000);
		}
	}
	
	delete machine;
	return 0;
}