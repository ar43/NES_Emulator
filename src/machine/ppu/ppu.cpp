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
			registers.ppustatus.SetBit(StatusBits::SPRITE0_HIT,false);
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
			registers.ppustatus.SetBit(StatusBits::VBLANK,false);
		}

	}
}