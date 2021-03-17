#include "ppu.h"
#include "../../utility/utility.h"
#include "../../logger/logger.h"
#include "../bus/bus.h"

void Ppu::Step(Bus *bus, uint16_t budget)
{
	budget *= 3;
	cycle += budget;
	if (cycle >= 341)
	{
		if (scanline <= 239)
		{
			if (scanline == 0)
			{
				display.RenderStart(bus);
				
			}
			uint8_t y_scroll = registers.ppuscroll.addr[1]; //not quite accurate... its not supposed to change in a frame unless writing to ppuaddr, hopefully this doesnt bug things
			uint8_t x_scroll = registers.ppuscroll.addr[0];
			int nametable = registers.ppuctrl.GetNametable(registers.v);
			//logger::PrintLine(logger::LogType::DEBUG, std::to_string(nametable));
			uint8_t bank = registers.ppuctrl.IsBitSet(ControllerBits::BACKGROUND_PATTERN);

			if (scanline <= 239 - y_scroll)
			{
				display.DrawBackgroundLineHSA(bus, x_scroll, y_scroll, nametable, bank, scanline,registers.ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND),registers.ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND_LEFT));
				if(x_scroll)
					display.DrawBackgroundLineHSB(bus, x_scroll, y_scroll, nametable, bank, scanline,registers.ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND),registers.ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND_LEFT));
			}
			else
			{
				display.DrawBackgroundLineVSB(bus, x_scroll, y_scroll, nametable, bank, scanline,registers.ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND),registers.ppumask.IsBitSet(MaskBits::SHOW_BACKGROUND_LEFT)); //todo: hor + ver scrolling if(x_scroll) DrawBackGroundLineVSA
			}
			if(scanline)
				display.DrawSprites(&registers,oam_data, scanline);
		}

		cycle -= 341;
		scanline++;

		
		
		if (scanline == 241)
		{
			registers.ppustatus.SetBit(StatusBits::VBLANK,true);
			if (registers.ppuctrl.IsBitSet(ControllerBits::GEN_NMI))
			{
				bus->nmi_pending = true;
			}
			else if (*force_render)
			{
				display.Render(&registers,oam_data);
			}
		}
		else if (scanline >= 262)
		{
			scanline = 0;
			bus->nmi_pending = false;
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
	registers.ppuaddr.w = &registers.w;
	registers.ppuaddr.scrolladdr = registers.ppuscroll.addr;
	registers.ppuaddr.x = &registers.x;
	registers.ppuctrl.Set(0,&registers.v);
	registers.ppudata.Set(0);
	registers.ppumask.Set(0);
	registers.ppuscroll.Clear();
	registers.ppuscroll.w = &registers.w;
	registers.ppuscroll.x = &registers.x;
	registers.ppuscroll.t = &registers.t;
	registers.ppustatus.Set(0);
	registers.v = 0;
	registers.t = 0;
	registers.w = 0;
}

uint8_t Ppu::ReadRegisters(Bus* bus, size_t loc)
{
	//mirroring
	if (loc >= 0x2008 && loc <= 0x3FFF)
	{
		loc &= 0x2007;
	}

	switch (loc)
	{
		case (size_t)ConstAddr::PPUDATA:
		{
			uint8_t ret = 0;
			auto ppudata = &registers.ppudata;
			auto ppuaddr = &registers.ppuaddr;
			auto ppuctrl = &registers.ppuctrl;

			if (ppuaddr->GetAddr() >= 0x3F00)
			{
				ret = bus->ReadPPU(ppuaddr->GetAddr());
				ppudata->Set(bus->ReadPPU(ppuaddr->GetAddr()-0x1000));
			}
			else
			{
				ret = ppudata->Get();
				ppudata->Set(bus->ReadPPU(ppuaddr->GetAddr()));
			}

			if (ppuctrl->IsBitSet(ControllerBits::VRAM_INC))
				ppuaddr->Add(32);
			else
				ppuaddr->Add(1);
			
			return ret;
		}
		case (size_t)ConstAddr::PPUSTATUS:
		{
			auto ppuaddr = &registers.ppuaddr;
			auto ppuscroll = &registers.ppuscroll;
			auto ppustatus = &registers.ppustatus;
			registers.w = 0;
			uint8_t ret = ppustatus->Get();
			ppustatus->SetBit(StatusBits::VBLANK,false);
			return ret;
		}
		case (size_t)ConstAddr::OAMDATA:
		{
			return oam_data[registers.oamaddr];
		}
		default:
		{
			//logger::PrintLine(logger::LogType::FATAL_ERROR, "Ppu::ReadRegisters - bad addr" + utility::int_to_hex(loc));
			return 0;
		}
	}
}

void Ppu::WriteRegisters(Bus *bus, size_t loc, uint8_t byte)
{
	//mirroring
	if (loc >= 0x2008 && loc <= 0x3FFF)
	{
		loc &= 0x2007;
	}

	switch (loc)
	{
		case (size_t)ConstAddr::PPUADDR:
		{
			registers.ppuaddr.Write(byte, &registers.t, &registers.v);
			break;
		}
		case (size_t)ConstAddr::PPUDATA:
		{
			auto ppuaddr = &registers.ppuaddr;
			auto ppuctrl = &registers.ppuctrl;
			bus->WritePPU(ppuaddr->GetAddr(), byte);
			if (ppuctrl->IsBitSet(ControllerBits::VRAM_INC))
				ppuaddr->Add(32);
			else
				ppuaddr->Add(1);
			break;
		}
		case (size_t)ConstAddr::PPUCTRL:
		{
			auto ppuctrl = &registers.ppuctrl;
			auto ppustatus = &registers.ppustatus;
			auto before = ppuctrl->IsBitSet(ControllerBits::GEN_NMI);
			ppuctrl->Set(byte, &registers.v);
			auto after = ppuctrl->IsBitSet(ControllerBits::GEN_NMI);
			if (before == false && after == true && ppustatus->IsBitSet(StatusBits::VBLANK))
			{
				bus->nmi_pending = true;
			}
			break;
		}
		case (size_t)ConstAddr::PPUMASK:
		{
			registers.ppumask.Set(byte);
			break;
		}
		case (size_t)ConstAddr::OAMADDR:
		{
			registers.oamaddr = byte;
			break;
		}
		case (size_t)ConstAddr::OAMDATA:
		{
			oam_data[registers.oamaddr] = byte;
			registers.oamdata = byte;
			registers.oamaddr++;
			break;
		}
		case (size_t)ConstAddr::PPUSCROLL:
		{
			registers.ppuscroll.Write(byte);
			break;
		}
		case (size_t)ConstAddr::OAMDMA:
		{
			registers.oamdma = byte;
			int addr = byte << 8;
			int oamaddr = registers.oamaddr;
			bus->add_dma_cycles = true;
			for (int i = 0; i < 256; i++)
			{
				oam_data[(oamaddr + i) & 0xFF] = bus->ReadCPUSafe(addr + i);
			}
			break;
		}
		default:
		{
			logger::PrintLine(logger::LogType::FATAL_ERROR, "Bad addr in PPU::WriteRegister");
			break;
		}

	}

}