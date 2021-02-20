#include "memory.h"
#include "../../logger/logger.h"
#include "../misc/nes_data.h"
#include "../../utility/utility.h"
#include <assert.h>

void Memory::WriteCPU(size_t loc, uint8_t byte)
{
	if (loc < 0 || loc >= cpu_data.size())
	{ 
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Memory::WriteCPU - Tried to write out of bounds: " + std::to_string(loc));
		return;
	}

	//mirroring
	if (loc >= 0x0800 && loc <= 0x1FFF)
	{
		loc &= 0x07FF;
	}
	else if (loc >= 0x2008 && loc <= 0x3FFF)
	{
		loc &= 0x2007;
	}

	cpu_data[loc] = byte;
}

uint8_t Memory::ReadCPU(size_t loc)
{
	if (loc < 0 || loc >= cpu_data.size())
	{ 
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Memory::ReadCPU - Tried to read out of bounds: " + utility::int_to_hex(loc));
		return 0;
	}

	//mirroring
	if (loc >= 0x0800 && loc <= 0x1FFF)
	{
		loc &= 0x07FF;
	}
	else if (loc >= 0x2008 && loc <= 0x3FFF)
	{
		loc &= 0x2007;
	}

	return cpu_data[loc];
}

void Memory::WritePPU(size_t loc, uint8_t byte)
{
	if (loc < 0 || loc >= ppu_data.size())
	{ 
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Memory::WritePPU - Tried to write out of bounds: " + std::to_string(loc));
		return;
	}

	//mirroring
	if (loc >= 0x3000 && loc <= 0x3EFF)
	{
		loc &= 0x2EFF;
	}
	else if (loc >= 0x3f20 && loc <= 0x3FFF)
	{
		loc &= 0x3f1f;
	}

	ppu_data[loc] = byte;
}

uint8_t Memory::ReadPPU(size_t loc)
{
	if (loc < 0 || loc >= ppu_data.size())
	{ 
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Memory::ReadPPU - Tried to read out of bounds: " + utility::int_to_hex(loc));
		return 0;
	}

	//mirroring
	if (loc >= 0x3000 && loc <= 0x3EFF)
	{
		loc &= 0x2EFF;
	}
	else if (loc >= 0x3f20 && loc <= 0x3FFF)
	{
		loc &= 0x3f1f;
	}

	return ppu_data[loc];
}

bool Memory::LoadNES(NesData *nes_data)
{
	mapper.number = nes_data->header.mapper_num;
	
	//this should probably be made with functions... this can get really messy with many mappers
	switch (mapper.number)
	{

	case 0:
	{
		mapper.name = "NROM";
		int start_prg_1 = 0x8000;
		int start_prg_2 = 0xC000;
		assert(nes_data->prg_rom.size() <= 2);
		assert(nes_data->chr_rom.size() == 1);
		char* dat_chr = nes_data->chr_rom.at(0).get();
		if (nes_data->prg_rom.size() == 1)
		{
			char* dat = nes_data->prg_rom.at(0).get();
			for (int i = 0; i < INES_PRG_BLOCK_SIZE; i++)
			{
				cpu_data[start_prg_1 + i] = dat[i];
				cpu_data[start_prg_2 + i] = dat[i];
			}
		}
		else
		{
			char* dat = nes_data->prg_rom.at(0).get();
			char* dat2 = nes_data->prg_rom.at(1).get();
			for (int i = 0; i < INES_PRG_BLOCK_SIZE; i++)
			{
				cpu_data[start_prg_1 + i] = dat[i];
				cpu_data[start_prg_2 + i] = dat2[i];
			}
		}
		for (int i = 0; i < INES_CHR_BLOCK_SIZE; i++)
		{
			ppu_data[i] = dat_chr[i];
		}
		break;
	}
	default :
	{
		logger::PrintLine(logger::LogType::ERROR, "Memory::LoadNES - Unsupported mapper");
		return false;
	}

	}

	int initial_pc = cpu_data[0xFFFD]*256+cpu_data[0xFFFC];
	logger::PrintLine(logger::LogType::INFO, "Mapper name: " + mapper.name + " Program start would be: " + utility::int_to_hex(initial_pc));

	return true;
}