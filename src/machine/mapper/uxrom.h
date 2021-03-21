#pragma once
#include "mapper.h"
#include <vector>
#include <memory>
#include <cassert>
class Uxrom: public Mapper
{
public:
	Uxrom(int nametable_mirroring,std::vector<std::shared_ptr<char>> vec, int num_banks, uint8_t *chr_rom) 
	{
		this->number = 2;
		this->name = "UXROM";
		this->nametable_mirroring = nametable_mirroring;
		this->num_banks = num_banks;
		assert(num_banks <= 16);
		if (chr_rom != nullptr)
		{
			memcpy(this->chr_rom, chr_rom, 0x2000);
			this->use_chr_ram = false;
		}
		else
		{
			this->use_chr_ram = true;
		}
		for (int i = 0; i < num_banks; i++)
		{
			memcpy(this->prg_rom[i], (uint8_t*)vec.at(i).get(), 0x4000);
		}
			
	}

	int num_banks = 0;
	void WritePRG(size_t loc, uint8_t byte);
	uint8_t ReadPRG(size_t loc);
	bool WritePPU(size_t loc, uint8_t byte);
	int ReadPPU(size_t loc);
private:
	uint8_t prg_rom[16][0x4000] = { 0 };
	uint8_t chr_rom[0x2000] = { 0 };
	uint8_t chr_ram[0x2000] = { 0 };
	int current_bank = 0;
};