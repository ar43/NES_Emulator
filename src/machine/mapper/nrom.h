#pragma once
#include "mapper.h"
#include <memory>

class Nrom: public Mapper
{
public:
	Nrom(int nametable_mirroring, uint8_t *prg_rom1, uint8_t *prg_rom2, uint8_t *chr_rom) 
	{
		this->number = 0;
		this->name = "NROM";
		this->mirror_prg_rom = mirror_prg_rom;
		this->use_chr_ram = chr_ram;
		this->nametable_mirroring = nametable_mirroring;
		memcpy(this->prg_rom, prg_rom1, 0x4000);
		if (prg_rom2 != nullptr)
		{
			memcpy(this->prg_rom+0x4000, prg_rom2, 0x4000);
			this->mirror_prg_rom = false;
		}
		else
		{
			memset(this->prg_rom+0x4000, 0, 0x4000);
			this->mirror_prg_rom = true;
		}
		if (chr_rom != nullptr)
		{
			memcpy(this->chr_rom, chr_rom, 0x2000);
			this->use_chr_ram = false;
		}
		else
		{
			this->use_chr_ram = true;
		}
			
		memset(this->chr_ram, 0, 0x2000);
		memset(this->prg_ram, 0, 0x2000);
	}
	bool mirror_prg_rom;

	void WritePRG(size_t loc, uint8_t byte);
	uint8_t ReadPRG(size_t loc);
	bool WritePPU(size_t loc, uint8_t byte);
	int ReadPPU(size_t loc);
private:
	uint8_t prg_rom[0x8000];
	uint8_t prg_ram[0x2000];
	uint8_t chr_rom[0x2000];
	uint8_t chr_ram[0x2000];

};