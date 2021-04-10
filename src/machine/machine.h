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
#include "user_interface/user_interface.h"
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
	MachineStatus status;
	UserInterface ui;

	Frame frame;
	void Run();
	void Init();

private:
	
	void LoadROM(std::string path);
	void UnloadROM();

	bool ParseINES(std::string path);
	void PollInterrupts();

	bool LoadCartridge(NesData* nes_data);
	void InitStatus();
};