#include "mmc1.h"
#include "../../utility/utility.h"

void Mmc1::WritePRG(size_t loc, uint8_t byte)
{
	if (loc >= 0x6000 && loc <= 0x7FFF)
	{
		prg_ram[loc - 0x6000] = byte;
	}
	else if (loc >= 0x8000 && loc <= 0xFFFF)
	{
		if (utility::IsBitSet(byte, 7))
		{
			control_register |= 0x0C;
			shift_register.Clear();
		}
		else
		{
			shift_register.value = ((shift_register.value >> 1) | ((byte & 1) << 4)) & 0b11111;
			shift_register.counter++;

			if (shift_register.counter == 5)
			{
				if (loc <= 0x9FFF)
				{
					control_register = shift_register.value;
					nametable_mirroring = control_register & 3;
				}
				else if (loc <= 0xBFFF)
				{
					chr0_register = shift_register.value;
					*rebuild_pixels = true;
				}
				else if (loc <= 0xDFFF)
				{
					chr1_register = shift_register.value;
					*rebuild_pixels = true;
				}
				else if (loc <= 0xFFFF)
				{
					prg_register = shift_register.value;
					//if ((prg_register & 0b1111) > num_banks_prg - 1)
					//logger::PrintLine(logger::LogType::FATAL_ERROR, "prg_register out of bounds");
				}
				shift_register.Clear();
			}
		}
	}
}

uint8_t Mmc1::ReadPRG(size_t loc)
{
	if (loc >= 0x6000 && loc <= 0x7FFF)
	{
		return prg_ram[loc - 0x6000];
	}
	const int prg_bank_mode = (control_register & 0b1100) >> 2;
	switch (prg_bank_mode)
	{
		case 0:
		{
			if (loc >= 0x8000 && loc <= 0xBFFF)
			{
				return prg_rom[prg_register&0b1110][loc - 0x8000];
			}
			else if (loc >= 0xC000 && loc <= 0xFFFF)
			{
				return prg_rom[(prg_register&0b1110)+1][loc - 0xC000];
			}
			break;
		}
		case 1:
		{
			if (loc >= 0x8000 && loc <= 0xBFFF)
			{
				return prg_rom[prg_register&0b1110][loc - 0x8000];
			}
			else if (loc >= 0xC000 && loc <= 0xFFFF)
			{
				return prg_rom[(prg_register&0b1110)+1][loc - 0xC000];
			}
			break;
		}
		case 2:
		{
			if (loc >= 0x8000 && loc <= 0xBFFF)
			{
				return prg_rom[0][loc - 0x8000];
			}
			else if (loc >= 0xC000 && loc <= 0xFFFF)
			{
				return prg_rom[prg_register&0b1111][loc - 0xC000];
			}
			break;
		}
		case 3:
		{
			if (loc >= 0x8000 && loc <= 0xBFFF)
			{
				return prg_rom[(prg_register&0b1111)][loc - 0x8000];
			}
			else if (loc >= 0xC000 && loc <= 0xFFFF)
			{
				return prg_rom[num_banks_prg-1][loc - 0xC000];
			}
			break;
		}
		default:
		{
			logger::PrintLine(logger::LogType::FATAL_ERROR, "Mapper: Illegal prg bank mode");
			break;
		}
	}
	logger::PrintLine(logger::LogType::FATAL_ERROR, "Mapper: Illegal mem");
	return 0;
}

bool Mmc1::WritePPU(size_t loc, uint8_t byte)
{
	if (use_chr_ram && loc < 0x2000)
	{
		chr_ram[loc] = byte;
		return true;
	}
	return false;
}

int Mmc1::ReadPPU(size_t loc)
{
	if (loc < 0x2000)
	{
		if (!use_chr_ram)
		{
			bool chr_mode = utility::IsBitSet(control_register, 4);
			if (!chr_mode)
			{
				if (loc <= 0x0FFF)
				{
					return chr_rom[chr0_register & 0x1E][loc];
				}
				else
				{
					return chr_rom[(chr0_register & 0x1E)+1][loc-0x1000];
				}
			}
			else
			{
				if (loc <= 0x0FFF)
				{
					return chr_rom[chr0_register][loc];
				}
				else
				{
					return chr_rom[chr1_register][loc-0x1000];
				}
			}
		}
		else
		{
			return chr_ram[loc];
		}
		
		
	}
	return -1;
}
