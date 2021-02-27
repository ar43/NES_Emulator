#include "ppu.h"
#include "../../utility/utility.h"
#include "../../logger/logger.h"

void Ppu::Step(Memory *mem, uint16_t budget)
{
	budget *= 3;
	cycle += budget;
	if (cycle >= 341)
	{
		if (IsSprite0Hit(mem))
		{
			registers.ppustatus.SetBit(StatusBits::SPRITE0_HIT,true);
			mem->sprite0_hit_y = mem->oam_data[0];
		}
		/*if (registers.ppuscroll.changed)
		{
			logger::PrintLine(logger::LogType::DEBUG, "New Scroll: x: " + std::to_string(registers.ppuscroll.addr[0]) + " y: " + std::to_string(registers.ppuscroll.addr[1]) + "line: " + std::to_string(scanline));
			registers.ppuscroll.changed = false;
		}*/
		cycle -= 341;
		scanline++;
		
		if (scanline == 241)
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
			registers.ppustatus.SetBit(StatusBits::SPRITE_OVERFLOW,false);
			registers.ppustatus.SetBit(StatusBits::VBLANK,false);
		}

	}
}

bool Ppu::IsSprite0Hit(Memory *mem)
{
	int y = mem->oam_data[0];
	int x = mem->oam_data[3];
	return (y == scanline) && x <= cycle && registers.ppumask.IsBitSet(MaskBits::SHOW_SPRITES);
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