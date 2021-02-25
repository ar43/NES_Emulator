#pragma once
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "ppu/ppu.h"
#include "misc/nes_data.h"
#include "../timing/frame.h"
#include "misc/machine_status.h"
#include "input/input.h"
#include <memory>

class Machine
{
public:
	Machine();
	Cpu cpu;
	Memory memory;
	Ppu ppu;
	Input input;
	std::unique_ptr<NesData> nes_data;
	MachineStatus machine_status;

	Frame frame;

	void Init();
	bool LoadNES(std::string path);
	void PollInterrupts();
	void RunROM(std::string path);
private:
	void Run();
};