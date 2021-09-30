#pragma once
#include "mapper.h"

class Mmc3: public Mapper
{
public:
	Mmc3(int nametable_mirroring, std::vector<std::shared_ptr<char>> prg_rom, int num_banks_prg, std::vector<std::shared_ptr<char>> chr_rom, int num_banks_chr, bool battery, bool *rebuild_pixels) 
	{
		this->number = 4;
		this->name = "MMC3";
		this->nametable_mirroring = nametable_mirroring;
		this->num_banks_prg = num_banks_prg*2;
		this->num_banks_chr = num_banks_chr*8;
		if (this->num_banks_prg > 64 || this->num_banks_chr > 256)
			logger::PrintLine(logger::LogType::FATAL_ERROR, "PRG / CHR data overflow");
		if (num_banks_chr > 0)
		{
			//memcpy(this->chr_rom, chr_rom, 0x2000);
			int cnt = 0;
			for (int i = 0; i < num_banks_chr; i++)
			{
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*0, 1024);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*1, 1024);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*2, 1024);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*3, 1024);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*4, 1024);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*5, 1024);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*6, 1024);
				cnt++;
				memcpy(this->chr_rom[cnt], (uint8_t*)chr_rom.at(i).get() + 1024*7, 1024);
				cnt++;
			}
			this->use_chr_ram = false;
		}
		else
		{
			this->use_chr_ram = true;
		}
		int cnt = 0;
		for (int i = 0; i < num_banks_prg; i++)
		{
			memcpy(this->prg_rom[cnt], (uint8_t*)prg_rom.at(i).get(), 0x2000);
			cnt++;
			memcpy(this->prg_rom[cnt], (uint8_t*)prg_rom.at(i).get() + 0x2000, 0x2000);
			cnt++;
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
	void SaveRAM(std::string md5);
	void LoadRAM(std::string md5);
	void TickIRQ();
	void ClearRegisters()
	{
		bank_select = 0;
		bank_data = 0;
		prg_ram_protect = 0;
		irq_latch = 0;
		memset(R, 0, sizeof(R));
	}
private:
	uint8_t prg_rom[64][0x2000] = { 0 };
	uint8_t chr_rom[256][1024] = { 0 };
	uint8_t chr_ram[0x2000] = { 0 };
	uint8_t prg_ram[0x2000] = { 0 };

	uint8_t bank_select = 0;
	uint8_t bank_data = 0;
	uint8_t prg_ram_protect = 0;

	uint8_t irq_latch = 0;
	bool irq_reload = false;
	uint8_t irq_counter = 0;

	uint8_t R[8] = { 0 };
};