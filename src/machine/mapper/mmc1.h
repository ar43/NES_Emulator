#pragma once
#include "mapper.h"

struct ShiftRegister
{
	void Clear()
	{
		counter = 0;
		value = 0b10000;
	}
	uint8_t counter = 0;
	uint8_t value = 0b10000;
};

class Mmc1: public Mapper
{
public:
	Mmc1(int nametable_mirroring, std::vector<std::shared_ptr<char>> prg_rom, int num_banks_prg, std::vector<std::shared_ptr<char>> chr_rom, int num_banks_chr, bool battery, bool *rebuild_pixels) 
	{
		this->number = 1;
		this->name = "MMC1";
		this->nametable_mirroring = nametable_mirroring;
		this->num_banks_prg = num_banks_prg;
		this->num_banks_chr = num_banks_chr*2;
		if (this->num_banks_prg > 32 || this->num_banks_chr > 32)
			logger::PrintLine(logger::LogType::FATAL_ERROR, "PRG / CHR data overflow");
		if (num_banks_chr > 0)
		{
			//memcpy(this->chr_rom, chr_rom, 0x2000);
			int cnt = 0;
			for (int i = 0; i < num_banks_chr; i++)
			{
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get(), 0x1000);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 0x1000, 0x1000);
				cnt++;
			}
			this->use_chr_ram = false;
		}
		else
		{
			this->use_chr_ram = true;
		}
		for (int i = 0; i < num_banks_prg; i++)
		{
			memcpy(this->prg_rom[i], (uint8_t*)prg_rom.at(i).get(), 0x4000);
		}
		this->battery = battery;
		this->rebuild_pixels = rebuild_pixels;
		//this->control_register |= 0x0C;
	}
	int num_banks_prg = 0;
	int num_banks_chr = 0;
	void WritePRG(size_t loc, uint8_t byte);
	uint8_t ReadPRG(size_t loc);
	bool WritePPU(size_t loc, uint8_t byte);
	int ReadPPU(size_t loc);
	bool* rebuild_pixels;
	void SaveRAM(std::string hash);
	void LoadRAM(std::string hash);
	void ClearRegisters()
	{
		control_register = 0x0C;
		chr0_register = 0;
		chr1_register = 0;
		prg_register = 0;
		shift_register.Clear();
	}
private:
	uint8_t prg_rom[32][0x4000] = { 0 };
	uint8_t chr_rom[32][0x1000] = { 0 };
	uint8_t chr_ram[0x2000] = { 0 };
	uint8_t prg_ram[0x2000] = { 0 };

	ShiftRegister shift_register;
	uint8_t control_register = 0x0C;
	uint8_t chr0_register = 0;
	uint8_t chr1_register = 0;
	uint8_t prg_register = 0;
};