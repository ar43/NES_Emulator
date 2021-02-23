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
	memory.AttachPPURegisters(&ppu.registers);
}

void Machine::RunCPUTest(int instruction_count)
{
	logger::CPU_TEST_MODE = true;
	if (LoadNES("cpu_testing/nestest.nes"))
	{
		if (memory.LoadNES(nes_data.get()))
		{
			cpu.RunTest(&memory, instruction_count);
		}
	}
	logger::CPU_TEST_MODE = false;
}

void Machine::RunROM(std::string path, Display *display)
{
	if (LoadNES(path))
	{
		if (memory.LoadNES(nes_data.get()))
		{
			Run(display);
		}
	}
}

void Machine::Run(Display *display)
{
	int initial_pc = memory.ReadCPU(0xFFFD)*256 + memory.ReadCPU(0xFFFC); //normally we jump to this
	cpu.registers[(size_t)RegId::PC]->set(initial_pc);
	uint32_t currentTime = SDL_GetTicks();
	int counter = 0;
	while (1)
	{
		static uint64_t cycle_accumulator = 0;

		uint64_t old_cycle = cpu.GetCycles();
		cpu.ExecuteInstruction(&memory, display);

		uint16_t budget = (uint16_t)(cpu.GetCycles() - old_cycle);
		ppu.Step(&memory,budget);

		cycle_accumulator += budget;
		if (cycle_accumulator >= 29780)
		{
			cycle_accumulator -= 29780;
			uint32_t deltaTime = SDL_GetTicks() - currentTime;
			logger::PrintLine(logger::LogType::INFO, "Frame time: " + std::to_string(deltaTime));
			//SDL_Delay(1000/60);
			currentTime = SDL_GetTicks();
		}
		
	}
}

bool Machine::LoadNES(std::string path)
{
	std::size_t found = path.find(".nes");
	if (found == std::string::npos)
	{
		logger::PrintLine(logger::LogType::ERROR, "Bad file format! .nes required");
		return false;
	}

	std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
	if (!ifs.is_open())
	{
		logger::PrintLine(logger::LogType::ERROR, "File does not exist!");
		return false;
	}
	char header[16];
	ifs.read(header, 16);
	if (!ifs)
	{
		logger::PrintLine(logger::LogType::ERROR, "Bad file format! File too short.");
		ifs.close();
		return false;
	}
	if (strncmp(header, "NES\x1A", 4) != 0)
	{
		logger::PrintLine(logger::LogType::ERROR, "Bad file signature!");
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
			logger::PrintLine(logger::LogType::ERROR, "Bad file format! File too short.");
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
			logger::PrintLine(logger::LogType::ERROR, "Bad file format! File too short.");
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
			logger::PrintLine(logger::LogType::ERROR, "Bad file format! File too short.");
			ifs.close();
			return false;
		}
		nes_data->chr_rom.push_back(data);
	}

	ifs.close();
	return true;
}