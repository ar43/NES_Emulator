#pragma once
#include "cpu/cpu.h"
#include "memory/memory.h"

class Machine
{
public:
	Machine()
	{
		logger::PrintLine(logger::LogType::INFO, "Created machine");
	}

	Cpu cpu;
	Memory memory;
};