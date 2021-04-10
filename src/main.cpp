#include "logger/logger.h"
#include "machine/machine.h"

int main(int argc, char *args[])
{
	logger::PrintLine(logger::LogType::INFO, "Starting NES emulation");

	auto machine = new Machine();
	machine->Init();
	if (argc >= 3)
	{
		if (std::string(args[2]).compare(std::string("-force_render")) == 0)
			machine->status.force_render = true;
	}

	if(argc >= 2)
		machine->status.pending_rom = std::string(args[1]);

	machine->Run();
	
	delete machine;
	return 0;
}