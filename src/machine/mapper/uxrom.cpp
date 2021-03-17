#include "uxrom.h"
#include "../../utility/utility.h"

void Uxrom::WritePRG(size_t loc, uint8_t byte)
{
	if (loc >= 0x8000 && loc <= 0xFFFF)
	{
		current_bank = byte & 0x0f;
	}
}

uint8_t Uxrom::ReadPRG(size_t loc)
{
	if (loc >= 0x8000 && loc <= 0xBFFF)
	{
		return prg_rom[current_bank][loc - 0x8000];
	}
	else if (loc >= 0xC000 && loc <= 0xFFFF)
	{
		return prg_rom[num_banks-1][loc - 0xC000];
	}
	else
	{
		//logger::PrintLine(logger::LogType::WARNING, "Reading illegal PRG");
		return 0;
	}
}

bool Uxrom::WritePPU(size_t loc, uint8_t byte)
{
	if (use_chr_ram && loc < 0x2000)
	{
		chr_ram[loc] = byte;
		return true;
	}
	return false;
}

int Uxrom::ReadPPU(size_t loc)
{
	if (loc < 0x2000)
	{
		if (!use_chr_ram)
		{
			return chr_rom[loc];
		}
		else
		{
			return chr_ram[loc];
		}
	}
	return -1;
}
