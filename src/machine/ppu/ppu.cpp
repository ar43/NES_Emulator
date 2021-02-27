#include "ppu.h"
#include "../../utility/utility.h"
#include "../../logger/logger.h"

void Ppu::Step(Memory *mem, uint16_t budget)
{
	budget *= 3;
	cycle += budget;
	if (cycle >= 341)
	{
		if (scanline <= 239)
		{
			if (!registers.ppustatus.IsBitSet(StatusBits::SPRITE0_HIT) && IsSprite0Hit(mem, scanline))
			{
				registers.ppustatus.SetBit(StatusBits::SPRITE0_HIT,true);
			}

			if (scanline == 0)
				display.RenderStart(mem);
			uint8_t x_scroll = mem->ppu_registers->ppuscroll.addr[0];
			int nametable = mem->ppu_registers->ppuctrl.GetNametable(mem->ppu_registers->v);
			//logger::PrintLine(logger::LogType::DEBUG, std::to_string(nametable));
			uint8_t bank = mem->ppu_registers->ppuctrl.IsBitSet(ControllerBits::BACKGROUND_PATTERN);
			display.DrawBackgroundLineHSA(mem, x_scroll, nametable, bank, scanline);
			display.DrawBackgroundLineHSB(mem, x_scroll, nametable, bank, scanline);
		}

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

bool Ppu::IsSprite0Hit(Memory *mem, int scanline)
{
	int y = mem->oam_data[0]+1;
	if (scanline > y + 7 || scanline < y || y-1 >= 0xEF)
		return false;
	int attributes = mem->oam_data[2];
	int x = mem->oam_data[3];
	uint8_t index = mem->oam_data[1];
	bool what = (y == scanline) && x <= cycle && registers.ppumask.IsBitSet(MaskBits::SHOW_SPRITES);
	uint8_t bank = mem->ppu_registers->ppuctrl.IsBitSet(ControllerBits::SPRITE_PATTERN);
	bool flip_h = utility::IsBitSet(attributes, 6);
	bool flip_v = utility::IsBitSet(attributes, 7);
	int i = scanline - y;
	uint32_t* pixels = (uint32_t*)display.surface->pixels;
	for (int j = 0; j < TILE_WIDTH; j++)
	{
		uint8_t value = mem->pixel_values[bank][index*PIXEL_PER_TILE + i*TILE_WIDTH+j];

		if (value == 0)
			continue;
		int loc = 0;

		if(!flip_h && !flip_v)
			loc = (y + i) * SCREEN_WIDTH + ((x + j) & 255);
		else if (flip_h && flip_v)
			loc = (y + (7 - i)) * SCREEN_WIDTH + ((x + (7 - j)) & 255);
		else if(flip_h)
			loc = (y + i) * SCREEN_WIDTH + ((x + (7 - j)) & 255);
		else if (flip_v)
			loc = (y + (7 - i)) * SCREEN_WIDTH + ((x + j) & 255);

		if ((pixels[loc] & 0xFF) != 0xFE)
		{
			return true;
		}
	}
	return false;
}

void Ppu::HandleReset()
{
	cycle = 0;
	scanline = 0;
	registers.oamaddr = 0;
	registers.oamdata = 0;
	registers.oamdma = 0;
	registers.ppuaddr.Clear();
	registers.ppuctrl.Set(0,&registers.v);
	registers.ppudata.Set(0);
	registers.ppumask.Set(0);
	registers.ppuscroll.Clear();
	registers.ppustatus.Set(0);
}