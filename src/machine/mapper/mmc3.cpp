#include "mmc3.h"
#include "../misc/constants.h"
#include "../../utility/utility.h"


void Mmc3::SaveRAM(std::string hash)
{
	if (battery)
	{
		std::ofstream(std::string(BATTERY_PATH) + "/" + hash + ".sav", std::ios::binary).write((const char*)prg_ram, sizeof(prg_ram));
		logger::PrintLine(logger::LogType::INFO, "Saved battery data");
	}
}

void Mmc3::LoadRAM(std::string hash)
{
	if (battery)
	{
		std::filesystem::create_directories(BATTERY_PATH);
		std::ifstream ifs(std::string(BATTERY_PATH) + "/" + hash + ".sav", std::ifstream::in | std::ifstream::binary);
		if (!ifs.is_open())
		{
			return;
		}
		ifs.read((char*)prg_ram, sizeof(prg_ram));
		logger::PrintLine(logger::LogType::INFO, "Loaded battery data");
	}
}

void Mmc3::WritePRG(size_t loc, uint8_t byte)
{
	if (loc >= 0x6000 && loc <= 0x7FFF)
	{
		prg_ram[loc - 0x6000] = byte;
	}
	else if (loc >= 0x8000 && loc <= 0x9FFE && ((loc & 1) == 0))
	{
		const int before = bank_select;
		bank_select = byte;

		if (utility::IsBitSet(before, 7) != utility::IsBitSet(bank_select, 7))
			*rebuild_pixels = true;
	}
	else if (loc >= 0x8001 && loc <= 0x9FFF && ((loc & 1) == 1))
	{
		const uint8_t reg_num = bank_select & 0b111;


		if (reg_num == 6 || reg_num == 7)
			byte = byte & 0b00111111;
		else if (reg_num == 0 || reg_num == 1)
			byte = byte & 0b11111110;

		if (reg_num <= 5)
			*rebuild_pixels = true;

		R[reg_num] = byte;
	}
	else if (loc >= 0xA000 && loc <= 0xBFFE && ((loc & 1) == 0))
	{
		byte = byte & 1;
		nametable_mirroring = byte + 2;
	}
	else if (loc >= 0xA001 && loc <= 0xBFFF && ((loc & 1) == 1))
	{
		//unimplemented and unimportant
		prg_ram_protect = byte;
	}
	else if (loc >= 0xC000 && loc <= 0xDFFE && ((loc & 1) == 0))
	{
		irq_latch = byte;
	}
	else if (loc >= 0xC001 && loc <= 0xDFFF && ((loc & 1) == 1))
	{
		irq_reload = true;
	}
	else if (loc >= 0xE000 && loc <= 0xFFFE && ((loc & 1) == 0))
	{
		irq_enabled = false;
		irq_pending = false;
	}
	else if (loc >= 0xE001 && loc <= 0xFFFF && ((loc & 1) == 1))
	{
		irq_enabled = true;
	}

}

uint8_t Mmc3::ReadPRG(size_t loc)
{
	if (loc >= 0x6000 && loc <= 0x7FFF)
	{
		return prg_ram[loc - 0x6000];
	}
	const bool prg_bank_mode = utility::IsBitSet(bank_select, 6);

	if (prg_bank_mode)
	{
		if (loc >= 0x8000 && loc <= 0x9FFF)
		{
			return prg_rom[num_banks_prg-2][loc - 0x8000];
		}
		else if (loc >= 0xA000 && loc <= 0xBFFF)
		{
			return prg_rom[R[7]][loc - 0xA000];
		}
		else if (loc >= 0xC000 && loc <= 0xDFFF)
		{
			return prg_rom[R[6]][loc - 0xC000];
		}
		else if (loc >= 0xE000 && loc <= 0xFFFF)
		{
			return prg_rom[num_banks_prg-1][loc - 0xE000];
		}
	}
	else
	{
		if (loc >= 0x8000 && loc <= 0x9FFF)
		{
			return prg_rom[R[6]][loc - 0x8000];
		}
		else if (loc >= 0xA000 && loc <= 0xBFFF)
		{
			return prg_rom[R[7]][loc - 0xA000];
		}
		else if (loc >= 0xC000 && loc <= 0xDFFF)
		{
			return prg_rom[num_banks_prg-2][loc - 0xC000];
		}
		else if (loc >= 0xE000 && loc <= 0xFFFF)
		{
			return prg_rom[num_banks_prg-1][loc - 0xE000];
		}
	}

	
	logger::PrintLine(logger::LogType::FATAL_ERROR, "Mapper: Illegal mem - " + utility::int_to_hex(loc));
	return 0;
}

bool Mmc3::WritePPU(size_t loc, uint8_t byte)
{
	if (use_chr_ram && loc < 0x2000)
	{
		chr_ram[loc] = byte;
		return true;
	}
	return false;
}

int Mmc3::ReadPPU(size_t loc)
{
	if (loc < 0x2000)
	{
		if (!use_chr_ram)
		{
			bool chr_mode = utility::IsBitSet(bank_select, 7);
			if (!chr_mode)
			{
				if (loc >= 0 && loc <= 0x03FF)
				{
					return chr_rom[R[0]][loc];
				}
				else if (loc >= 0x0400 && loc <= 0x07FF)
				{
					return chr_rom[R[0]+1][loc-0x0400];
				}
				else if (loc >= 0x0800 && loc <= 0x0BFF)
				{
					return chr_rom[R[1]][loc-0x0800];
				}
				else if (loc >= 0x0C00 && loc <= 0x0FFF)
				{
					return chr_rom[R[1]+1][loc-0x0C00];
				}
				else if (loc >= 0x1000 && loc <= 0x13FF)
				{
					return chr_rom[R[2]][loc-0x1000];
				}
				else if (loc >= 0x1400 && loc <= 0x17FF)
				{
					return chr_rom[R[3]][loc-0x1400];
				}
				else if (loc >= 0x1800 && loc <= 0x1BFF)
				{
					return chr_rom[R[4]][loc-0x1800];
				}
				else if (loc >= 0x1C00 && loc <= 0x1FFF)
				{
					return chr_rom[R[5]][loc-0x1C00];
				}
			}
			else
			{
				if (loc >= 0 && loc <= 0x03FF)
				{
					return chr_rom[R[2]][loc];
				}
				else if (loc >= 0x0400 && loc <= 0x07FF)
				{
					return chr_rom[R[3]][loc-0x0400];
				}
				else if (loc >= 0x0800 && loc <= 0x0BFF)
				{
					return chr_rom[R[4]][loc-0x0800];
				}
				else if (loc >= 0x0C00 && loc <= 0x0FFF)
				{
					return chr_rom[R[5]][loc-0x0C00];
				}
				else if (loc >= 0x1000 && loc <= 0x13FF)
				{
					return chr_rom[R[0]][loc-0x1000];
				}
				else if (loc >= 0x1400 && loc <= 0x17FF)
				{
					return chr_rom[R[0]+1][loc-0x1400];
				}
				else if (loc >= 0x1800 && loc <= 0x1BFF)
				{
					return chr_rom[R[1]][loc-0x1800];
				}
				else if (loc >= 0x1C00 && loc <= 0x1FFF)
				{
					return chr_rom[R[1]+1][loc-0x1C00];
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

void Mmc3::TickIRQ()
{
	if (irq_reload || irq_counter == 0)
	{
		irq_counter = irq_latch;
		irq_reload = false;
	}
	else
	{
		irq_counter--;
	}

	if (irq_counter == 0 && irq_enabled)
		irq_pending = true;
}