#pragma once
#include "ppuregisters.h"
#include "../memory/memory.h"
#include "display/display.h"

class Ppu
{
public:
	PpuRegisters registers;
	Display display;
	void Step(Memory *mem, uint16_t budget);
	void HandleReset();
	bool IsSprite0Hit(Memory *mem);
private:
	uint16_t cycle = 0;
	uint16_t scanline = 0;
};