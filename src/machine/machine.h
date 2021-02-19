#pragma once
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "misc/nes_data.h"
#include <memory>

class Machine
{
public:
	Machine();
	Cpu cpu;
	Memory memory;
	std::unique_ptr<NesData> nes_data;

	bool LoadNES(std::string path);
};