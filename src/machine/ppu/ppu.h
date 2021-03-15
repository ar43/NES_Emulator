#pragma once
#include "ppuregisters.h"
#include "display/display.h"

enum class ConstAddr
{
	PPUCTRL = 0x2000,
	PPUMASK,
	PPUSTATUS,
	OAMADDR,
	OAMDATA,
	PPUSCROLL,
	PPUADDR,
	PPUDATA,
	OAMDMA = 0x4014
};

class Bus;

class Ppu
{
public:

	Ppu() 
	{
		memset(data, 0, PPU_MEM_SIZE);
		memset(oam_data, 0, OAM_MEM_SIZE);
	}

	PpuRegisters registers;
	Display display;
	void Step(Bus *bus, uint16_t budget);
	void HandleReset();
	bool IsSprite0Hit(int scanline);

	void WriteRegisters(Bus *bus, size_t loc, uint8_t byte);
	uint8_t ReadRegisters(Bus *bus, size_t loc);

	uint8_t data[PPU_MEM_SIZE];
	uint8_t oam_data[OAM_MEM_SIZE];

	bool* force_render = nullptr;
private:
	uint16_t cycle = 0;
	uint16_t scanline = 0;
	
	
};