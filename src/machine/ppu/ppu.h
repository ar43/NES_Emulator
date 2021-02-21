#pragma once
#include "ppuregisters.h"
#include "../memory/memory.h"

class Ppu
{
public:
	PpuRegisters registers;
	void Step(Memory *mem, uint16_t budget);
private:
	uint16_t cycle = 0;
	uint16_t scanline = 0;
};