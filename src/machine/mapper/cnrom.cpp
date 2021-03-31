#include "cnrom.h"

void Cnrom::WritePRG(size_t loc, uint8_t byte)
{
	if (loc >= 0x8000 && loc <= 0xFFFF)
	{
		const int prev_bank = current_bank;
		current_bank = byte & (this->num_banks_chr-1); //ANDing to avoid out of bounds accesss
		if(prev_bank != current_bank)
			*rebuild_pixels = true;
	}
}

uint8_t Cnrom::ReadPRG(size_t loc)
{
	if (loc >= 0x8000 && loc <= 0xBFFF)
	{
		return prg_rom[0][loc - 0x8000];
	}
	else if (loc >= 0xC000 && loc <= 0xFFFF)
	{
		return prg_rom[1][loc - 0xC000];
	}
	else
	{
		//logger::PrintLine(logger::LogType::WARNING, "Reading illegal PRG");
		return 0;
	}
}

bool Cnrom::WritePPU(size_t loc, uint8_t byte)
{
	if (use_chr_ram && loc < 0x2000)
	{
		chr_ram[loc] = byte;
		return true;
	}
	return false;
}

int Cnrom::ReadPPU(size_t loc)
{
	if (loc < 0x2000)
	{
		if (!use_chr_ram)
		{
			return chr_rom[current_bank][loc];
		}
		else
		{
			return chr_ram[loc];
		}
	}
	return -1;
}
