#pragma once
#include "mapper.h"

class Cnrom: public Mapper
{
public:
	Cnrom(int nametable_mirroring,std::vector<std::shared_ptr<char>> prg_rom, std::vector<std::shared_ptr<char>> chr_rom, bool *rebuild_pixels) 
	{
		this->number = 3;
		this->name = "CNROM";
		this->nametable_mirroring = nametable_mirroring;
		this->num_banks_chr = chr_rom.size();
		const auto num_banks_prg = prg_rom.size();
		if (this->num_banks_chr > 256)
			logger::PrintLine(logger::LogType::FATAL_ERROR, "CHR data overflow");

		if (this->num_banks_chr)
		{
			for (int i = 0; i < this->num_banks_chr; i++)
			{
				memcpy(this->chr_rom[i], (uint8_t*)chr_rom.at(i).get(), 0x2000);
			}
			this->use_chr_ram = false;
		}
		else
		{
			this->use_chr_ram = true;
		}

		if (num_banks_prg == 1)
		{
			memcpy(this->prg_rom[0], (uint8_t*)prg_rom.at(0).get(), 0x4000);
			memcpy(this->prg_rom[1], (uint8_t*)prg_rom.at(0).get(), 0x4000);
			debugger_mirroring = true;
		}
		else if (num_banks_prg == 2)
		{
			memcpy(this->prg_rom[0], (uint8_t*)prg_rom.at(0).get(), 0x4000);
			memcpy(this->prg_rom[1], (uint8_t*)prg_rom.at(1).get(), 0x4000);
		}
		else
		{
			logger::PrintLine(logger::LogType::FATAL_ERROR, "PRG data overflow");
		}
		this->rebuild_pixels = rebuild_pixels;
	}
	bool* rebuild_pixels = nullptr;
	int num_banks_chr = 0;
	void WritePRG(size_t loc, uint8_t byte);
	uint8_t ReadPRG(size_t loc);
	bool WritePPU(size_t loc, uint8_t byte);
	int ReadPPU(size_t loc);
private:
	uint8_t prg_rom[2][0x4000] = { 0 };
	uint8_t chr_rom[256][0x2000] = { 0 };
	uint8_t chr_ram[0x2000] = { 0 };
	int current_bank = 0;
};