#include "nrom.h"

void Nrom::WritePRG(size_t loc, uint8_t byte)
{
	if (loc >= 0x6000 && loc <= 0x7FFF)
	{
		prg_ram[loc - 0x6000] = byte;
	}
}

uint8_t Nrom::ReadPRG(size_t loc)
{
	if (loc >= 0x6000 && loc <= 0x7FFF)
	{
		return prg_ram[loc - 0x6000];
	}
	else if (loc >= 0x8000 && loc <= 0xBFFF)
	{
		return prg_rom[loc - 0x8000];
	}
	else if (loc >= 0xC000 && loc <= 0xFFFF)
	{
		if (mirror_prg_rom)
			return prg_rom[loc - 0xC000];
		else
			return prg_rom[loc - 0x8000];
	}
	else
	{
		//logger::PrintLine(logger::LogType::WARNING, "Reading illegal PRG");
		return 0;
	}
}

bool Nrom::WritePPU(size_t loc, uint8_t byte)
{
	if (use_chr_ram && loc < 0x2000)
	{
		chr_ram[loc] = byte;
		return true;
	}
	return false;
}

int Nrom::ReadPPU(size_t loc)
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
