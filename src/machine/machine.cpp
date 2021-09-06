#include "machine.h"
#include "../logger/logger.h"
#include "../utility/utility.h"
#include "mapper/nrom.h"
#include "mapper/uxrom.h"
#include "mapper/cnrom.h"
#include "mapper/mmc1.h"
#include "mapper/mmc3.h"
#include <fstream>
#include <memory>
#include <filesystem>
#include <sstream>
#include <algorithm>

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)

Machine::Machine()
{
	logger::PrintLine(logger::LogType::INFO, "Created machine");
}

void Machine::Init()
{
	ui.Init();
	cpu.Init(&ui.debugger.debug_data, &ui.debugger.debug_mode);
	bus.AttachComponents(&cpu, input.joypad, &apu, &ppu);
	ppu.display.Init(ui.window.GetWindow(), ui.window.GetRenderer(), &ui.scale);
	apu.Init(&bus.irq_pending, &status.volume);
	ppu.force_render = &status.force_render;
	ui.debugger.machine_status = &this->status;
}

void Machine::LoadROM(std::string path)
{
	if (ParseINES(path))
	{
		if (LoadCartridge(nes_data.get()))
		{
			ppu.display.BuildPixelValues(&bus);
			status.running = RunningStatus::RUNNING_ROM;
			//Run();
		}
	}
	status.pending_rom.clear();
}

bool Machine::LoadCartridge(NesData *nes_data)
{
	switch (nes_data->header.mapper_num)
	{

	case 0:
	{
		int nametable_mirroring = 3;
		if (utility::IsBitSet(nes_data->header.flags6, 0))
			nametable_mirroring = 2;
		else if (utility::IsBitSet(nes_data->header.flags6, 3))
			nametable_mirroring = 999;

		assert(nes_data->header.PRG_ROM_size <= 2);
		assert(nes_data->header.CHR_ROM_size == 1 || nes_data->header.CHR_ROM_size == 0);
		char* dat1 = nullptr;
		char* dat2 = nullptr;
		char* dat_chr = nullptr;

		if (nes_data->header.PRG_ROM_size == 1)
		{
			dat1 = nes_data->prg_rom.at(0).get();
		}
		else
		{
			dat1 = nes_data->prg_rom.at(0).get();
			dat2 = nes_data->prg_rom.at(1).get();
		}

		if (nes_data->header.CHR_ROM_size == 1)
		{
			dat_chr = nes_data->chr_rom.at(0).get();
		}

		mapper = std::unique_ptr<Nrom>(new Nrom(nametable_mirroring, (uint8_t*)dat1, (uint8_t*)dat2, (uint8_t*)dat_chr));

		break;
	}
	case 1:
	{
		int nametable_mirroring = 3;
		if (utility::IsBitSet(nes_data->header.flags6, 0))
			nametable_mirroring = 2;
		else if (utility::IsBitSet(nes_data->header.flags6, 3))
			nametable_mirroring = 999;
		assert(nes_data->header.PRG_ROM_size > 0);

		mapper = std::unique_ptr<Mmc1>(new Mmc1(nametable_mirroring,nes_data->prg_rom,nes_data->header.PRG_ROM_size,nes_data->chr_rom,nes_data->header.CHR_ROM_size,utility::IsBitSet(nes_data->header.flags6,1),&bus.rebuild_pixels));

		break;
	}
	case 2:
	{
		int nametable_mirroring = 3;
		if (utility::IsBitSet(nes_data->header.flags6, 0))
			nametable_mirroring = 2;
		else if (utility::IsBitSet(nes_data->header.flags6, 3))
			nametable_mirroring = 999;
		assert(nes_data->header.PRG_ROM_size > 0 && nes_data->header.CHR_ROM_size <= 1);

		char* dat_chr = nullptr;
		if (nes_data->header.CHR_ROM_size == 1)
		{
			dat_chr = nes_data->chr_rom.at(0).get();
		}
		mapper = std::unique_ptr<Uxrom>(new Uxrom(nametable_mirroring,nes_data->prg_rom,nes_data->header.PRG_ROM_size,(uint8_t*)dat_chr));
		
		break;
	}
	case 3:
	{
		int nametable_mirroring = 3;
		if (utility::IsBitSet(nes_data->header.flags6, 0))
			nametable_mirroring = 2;
		else if (utility::IsBitSet(nes_data->header.flags6, 3))
			nametable_mirroring = 999;

		mapper = std::unique_ptr<Cnrom>(new Cnrom(nametable_mirroring,nes_data->prg_rom,nes_data->chr_rom,&bus.rebuild_pixels));

		break;
	}
	case 4:
	{
		int nametable_mirroring = 3;
		if (utility::IsBitSet(nes_data->header.flags6, 0))
			nametable_mirroring = 2;
		else if (utility::IsBitSet(nes_data->header.flags6, 3))
			nametable_mirroring = 999;
		assert(nes_data->header.PRG_ROM_size > 0);

		mapper = std::unique_ptr<Mmc3>(new Mmc3(nametable_mirroring,nes_data->prg_rom,nes_data->header.PRG_ROM_size,nes_data->chr_rom,nes_data->header.CHR_ROM_size,utility::IsBitSet(nes_data->header.flags6,1),&bus.rebuild_pixels));

		break;
	}
	default :
	{
		logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Memory::LoadNES - Unsupported mapper " + std::to_string(nes_data->header.mapper_num));
		return false;
	}

	}

	//int initial_pc = cpu_data[0xFFFD]*256+cpu_data[0xFFFC];
	bus.AttachMapper(mapper.get());
	mapper->LoadRAM(nes_data->file_name);
	if(mapper->GetNumber() != nes_data->header.mapper_num)
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Mapper number " + std::to_string(mapper->GetNumber()) + "does not match the header number " + std::to_string(nes_data->header.mapper_num));
	logger::PrintLine(logger::LogType::INFO, "Mapper name: " + mapper->name);

	ui.debugger.mapper = mapper->GetNumber();
	ui.debugger.debug_data.mirror = mapper->debugger_mirroring;

	return true;
}

void Machine::PollInterrupts()
{
	if (bus.nmi_pending)
	{
		ppu.display.Render();
		cpu.HandleNMI(&bus);
		bus.nmi_pending = false;
	}
	else if ((bus.irq_pending == true || *bus.dmcirq_pending == true || (mapper->irq_pending && mapper->irq_enabled)) && !cpu.registers[(size_t)RegId::P]->get_flag(flags::Flags::I))
	{
		cpu.HandleIRQ(&bus);
		//logger::PrintLine(logger::LogType::DEBUG, "IRQ request");
	}
	else if ((int)status.reset)
	{
		bus.WriteCPU(0x4015, 00); //silence the apu
		ppu.HandleReset();
		cpu.HandleReset(&bus, status.reset);
		apu.Reset();
		bus.Reset();
		status.reset = ResetType::NOT;
	}
}

void Machine::InitStatus()
{
	status.speedup = &frame.capTimer.bypass;
	apu.speedup = status.speedup;
	status.mute = &apu.mute;
}

void Machine::UnloadROM()
{
	mapper->SaveRAM(nes_data->file_name);
	status.reset = ResetType::HARD;
	ppu.display.Clear();
	SDL_SetWindowTitle(ui.window.GetWindow(), "NES Emulator (unloaded)");
}

void Machine::Run()
{
	frame.init();
	InitStatus();
	SDL_Delay(20);
	while ((int)status.running)
	{
		if (status.running == RunningStatus::RUNNING_ROM)
		{
			static uint64_t cycle_accumulator = 0;
			frame.start();

			while (status.paused)
			{
				input.Poll(&status, &ui);
				ui.UpdateAll();
				SDL_Delay(10);
			}

			if (frame.capTimer.tick(&frame))
			{
				input.Poll(&status, &ui);
				cycle_accumulator = 0;
				while (cycle_accumulator < 29780)
				{
					uint64_t old_cycle = cpu.GetCycles();
					PollInterrupts();
					cpu.ExecuteInstruction(&bus);
					uint16_t budget = (uint16_t)(cpu.GetCycles() - old_cycle);
					ppu.Step(&bus, mapper.get(), budget);
					apu.Step(&bus, budget);
					cycle_accumulator += budget;
				}
				apu.Play();
				ui.UpdateAll();
				frame.end(ui.window.GetWindow());

			}

			if (status.running != RunningStatus::RUNNING_ROM)
				UnloadROM();
		}
		else
		{
			input.Poll(&status, &ui);
			ui.UpdateAll();
			if (!status.pending_rom.empty())
				LoadROM(status.pending_rom);
			SDL_Delay(10);
		}
	}
}

bool Machine::ParseINES(std::string path)
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

	std::filesystem::path p(path);
	std::stringstream ss;
	ss << p.stem();
	nes_data->file_name = ss.str();
	nes_data->file_name.erase(std::remove(nes_data->file_name.begin(), nes_data->file_name.end(), '\"'), nes_data->file_name.end());
	if (nes_data->file_name.length() <= 0)
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Empty filename");
	std::transform(nes_data->file_name.begin(), nes_data->file_name.end(), nes_data->file_name.begin(), [](unsigned char c){ return std::tolower(c); }); //convert name to lowercase

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