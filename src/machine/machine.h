#pragma once
#include "cpu/cpu.h"
#include "bus/bus.h"
#include "ppu/ppu.h"
#include "misc/nes_data.h"
#include "../timing/frame.h"
#include "misc/machine_status.h"
#include "input/input.h"
#include "apu/apu.h"
#include "mapper/mapper.h"
#include <memory>

class Machine
{
public:
	Machine();
	Cpu cpu;
	Bus bus;
	Ppu ppu;
	Apu apu;
	std::unique_ptr<Mapper> mapper;
	Input input;
	std::unique_ptr<NesData> nes_data;
	MachineStatus machine_status;

	Frame frame;

	void Init();
	bool ParseINES(std::string path);
	void PollInterrupts();
	void RunROM(std::string path);
	bool LoadCartridge(NesData* nes_data);
	void InitStatus();
private:
	void Run();
};