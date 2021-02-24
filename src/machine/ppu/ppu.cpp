#include "ppu.h"
#include "../../utility/utility.h"

void Ppu::Step(Memory *mem, uint16_t budget)
{
	budget *= 3;
	cycle += budget;
	if (cycle >= 341)
	{
		cycle -= 341;
		scanline++;

		if (scanline == 30)
		{
			registers.ppustatus.SetBit(StatusBits::SPRITE0_HIT,true); //temporary hack
		}
		else if (scanline == 241)
		{
			registers.ppustatus.SetBit(StatusBits::VBLANK,true);
			if (registers.ppuctrl.IsBitSet(ControllerBits::GEN_NMI))
			{
				mem->trigger_nmi_interrupt = true;
			}
		}
		else if (scanline >= 262)
		{
			scanline = 0;
			mem->trigger_nmi_interrupt = false;
			registers.ppustatus.SetBit(StatusBits::SPRITE0_HIT,false);
			registers.ppustatus.SetBit(StatusBits::SPRITE_OVERFLOW,false);
			registers.ppustatus.SetBit(StatusBits::VBLANK,false);
		}

	}
}

void Ppu::HandleReset()
{
	cycle = 0;
	scanline = 0;
	registers.oamaddr = 0;
	registers.oamdata = 0;
	registers.oamdma = 0;
	registers.ppuaddr.Clear();
	registers.ppuctrl.Set(0);
	registers.ppudata.Set(0);
	registers.ppumask.Set(0);
	registers.ppuscroll.Clear();
	registers.ppustatus.Set(0);
}