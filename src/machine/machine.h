#pragma once
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "misc/nes_data.h"
#include "../timing/frame.h"
#include <memory>

class Machine
{
public:
	Machine();
	Cpu cpu;
	Memory memory;
	Ppu ppu;
	std::unique_ptr<NesData> nes_data;
	bool running = true;

	Frame frame;

	void Init();
	bool LoadNES(std::string path);
	void RunCPUTest(int instruction_count);
	void RunROM(std::string path);
private:
	void Run();
};