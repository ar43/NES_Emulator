#include "machine.h"
#include "../logger/logger.h"
#include "../utility/utility.h"
#include <fstream>
#include <memory>

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)

Machine::Machine()
{
	logger::PrintLine(logger::LogType::INFO, "Created machine");
}

void Machine::Init()
{
	memory.AttachStuff(&ppu.registers, input.joypad, &apu);
	ppu.display.Init();
	apu.Init(&memory.irq_pending);
}

void Machine::RunROM(std::string path)
{
	if (LoadNES(path))
	{
		if (memory.LoadNES(nes_data.get()))
		{
			memory.BuildPixelValues();
			Run();
		}
	}
}

void Machine::PollInterrupts()
{
	if (memory.nmi_pending)
	{
		ppu.display.Render(&memory);
		cpu.HandleNMI(&memory);
		memory.nmi_pending = false;
	}
	else if ((memory.irq_pending == true || *memory.dmcirq_pending == true) && !cpu.registers[(size_t)RegId::P]->get_flag(flags::Flags::I))
	{
		cpu.HandleIRQ(&memory);
		memory.irq_pending = false;
		logger::PrintLine(logger::LogType::DEBUG, "IRQ request");
	}
	else if (machine_status.reset)
	{
		memory.WriteCPU(0x4015, 00); //silence the apu
		ppu.HandleReset();
		cpu.HandleReset(&memory, machine_status.reset);
		apu.Reset();
		machine_status.reset = 0;
	}
}

void Machine::InitStatus()
{
	machine_status.speedup = &frame.capTimer.bypass;
	machine_status.mute = &apu.mute;
}

void Machine::Run()
{
	frame.init();
	InitStatus();
	SDL_Delay(20);
	while (machine_status.running)
	{
		static uint64_t cycle_accumulator = 0;
		frame.start();

		while (machine_status.paused)
		{
			input.PollPause(&machine_status);
		}

		if (frame.capTimer.tick(&frame))
		{
			input.Poll(&machine_status, ppu.display.GetWindow(), &ppu.display, &memory);
			cycle_accumulator = 0;
			while (cycle_accumulator < 29780)
			{
				uint64_t old_cycle = cpu.GetCycles();
				PollInterrupts();
				cpu.ExecuteInstruction(&memory);
				uint16_t budget = (uint16_t)(cpu.GetCycles() - old_cycle);
				ppu.Step(&memory, budget);
				apu.Step(&memory, budget);
				cycle_accumulator += budget;
			}
			apu.Play();
			frame.end(ppu.display.GetWindow());
			
		}
		
	}
}

bool Machine::LoadNES(std::string path)
{
	std::size_t found = path.find(".nes");
	if (found == std::string::npos)
	{
		path += ".nes";
	}
	

	std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
	if (!ifs.is_open())
	{
		logger::PrintLine(logger::LogType::INTERNAL_ERROR, "File does not exist!");
		return false;
	}
	char header[16];
	ifs.read(header, 16);
	if (!ifs)
	{
		logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Bad file format! File too short.");
		ifs.close();
		return false;
	}
	if (strncmp(header, "NES\x1A", 4) != 0)
	{
		logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Bad file signature!");
		ifs.close();
		return false;
	}
	nes_data = std::unique_ptr<NesData>(new NesData());
	nes_data->header.PRG_ROM_size = (uint8_t)header[4];
	nes_data->header.CHR_ROM_size = (uint8_t)header[5];
	nes_data->header.flags6 = (uint8_t)header[6];
	nes_data->header.flags7 = (uint8_t)header[7];
	nes_data->header.flags8 = (uint8_t)header[8];
	nes_data->header.flags9 = (uint8_t)header[9];
	nes_data->header.flags10 = (uint8_t)header[10];

	if ((nes_data->header.flags7 & 24) == 24)
	{
		nes_data->header.nes2 = true;
	}

	nes_data->header.mapper_num = HI_NIBBLE(nes_data->header.flags7) * 16 + HI_NIBBLE(nes_data->header.flags6);
	assert(nes_data->header.mapper_num >= 0 && nes_data->header.mapper_num <= 255);

	if (utility::IsBitSet(nes_data->header.flags6, (int)Flags6::TRAINER))
	{
		logger::PrintLine(logger::LogType::WARNING, "iNES file has TRAINER bit enabled.");
		ifs.read(nes_data->trainer_data.data(), nes_data->trainer_data.size());
		if (!ifs)
		{
			logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Bad file format! File too short.");
			ifs.close();
			return false;
		}

	}

	for (int i = 0; i < nes_data->header.PRG_ROM_size;i++)
	{
		std::shared_ptr<char> data(new char[INES_PRG_BLOCK_SIZE]);
		ifs.read(data.get(), INES_PRG_BLOCK_SIZE);
		if (!ifs)
		{
			logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Bad file format! File too short.");
			ifs.close();
			return false;
		}
		nes_data->prg_rom.push_back(data);
	}
	for (int i = 0; i < nes_data->header.CHR_ROM_size;i++)
	{
		std::shared_ptr<char> data(new char[INES_CHR_BLOCK_SIZE]);
		ifs.read(data.get(), INES_CHR_BLOCK_SIZE);
		if (!ifs)
		{
			logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Bad file format! File too short.");
			ifs.close();
			return false;
		}
		nes_data->chr_rom.push_back(data);
	}

	ifs.close();
	return true;
}