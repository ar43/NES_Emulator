#include "memory.h"
#include "../../logger/logger.h"
#include "../misc/nes_data.h"
#include "../../utility/utility.h"
#include "../input/joypad.h"
#include "../apu/apu.h"
#include <assert.h>

int lentable[] = { 10,254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30 };

void Memory::WriteCPU(size_t loc, uint8_t byte)
{
	//mirroring
	if (loc >= 0x0800 && loc <= 0x1FFF)
	{
		loc &= 0x07FF;
	}
	else if (loc >= 0x2008 && loc <= 0x3FFF)
	{
		loc &= 0x2007;
	}

	if (loc == (size_t)ConstAddr::PPUADDR)
	{
		ppu_registers->ppuaddr.Write(byte, &ppu_registers->t, &ppu_registers->v);
	}
	else if (loc == (size_t)ConstAddr::PPUDATA)
	{
		//logger::PrintLine(logger::LogType::WARNING, "Writing to PPUDATA! Who knows if this is correct?");
		auto ppudata = &ppu_registers->ppudata;
		auto ppuaddr = &ppu_registers->ppuaddr;
		auto ppuctrl = &ppu_registers->ppuctrl;
		//WritePPU(ppuaddr.GetAddr(),ppudata.Get());
		WritePPU(ppuaddr->GetAddr(), byte);
		if (ppuctrl->IsBitSet(ControllerBits::VRAM_INC))
			ppuaddr->Add(32);
		else
			ppuaddr->Add(1);
	}
	else if (loc == (size_t)ConstAddr::PPUCTRL)
	{
		auto ppuctrl = &ppu_registers->ppuctrl;
		auto ppustatus = &ppu_registers->ppustatus;
		auto before = ppuctrl->IsBitSet(ControllerBits::GEN_NMI);
		ppuctrl->Set(byte, &ppu_registers->v);
		auto after = ppuctrl->IsBitSet(ControllerBits::GEN_NMI);
		if (before == false && after == true && ppustatus->IsBitSet(StatusBits::VBLANK))
		{
			trigger_nmi_interrupt = true;
		}
	}
	else if (loc == (size_t)ConstAddr::PPUMASK)
	{
		ppu_registers->ppumask.Set(byte);
	}
	else if (loc == (size_t)ConstAddr::OAMADDR)
	{
		ppu_registers->oamaddr = byte;
	}
	else if (loc == (size_t)ConstAddr::OAMDATA)
	{
		oam_data[ppu_registers->oamaddr] = byte;
		ppu_registers->oamdata = byte;
		ppu_registers->oamaddr++;
	}
	else if (loc == (size_t)ConstAddr::PPUSCROLL)
	{
		ppu_registers->ppuscroll.Write(byte);
	}
	else if (loc == (size_t)ConstAddr::OAMDMA)
	{
		ppu_registers->oamdma = byte;
		int addr = byte << 8;
		int oamaddr = ppu_registers->oamaddr;
		add_dma_cycles = true;
		for (int i = 0; i < 256; i++)
		{
			oam_data[(oamaddr + i) & 0xFF] = cpu_data[addr + i];
		}
	}
	else if (loc == 0x4016)
	{
		joypad[0].Write(byte);
		joypad[1].Write(byte);
	} //sound below
	/*else if (loc == 0x4010)
	{
		if (utility::IsBitSet(byte, 7))
		{
			trigger_irq_interrupt = true;
		}
		else
		{
			trigger_irq_interrupt = false;
		}
	}*/
	else if (loc == 0x4000)
	{
		apu->pulse_channel[0].duty_cycle = (byte >> 6) & 3;
		apu->pulse_channel[0].length_counter_halt = utility::IsBitSet(byte, 5);
		apu->pulse_channel[0].c = utility::IsBitSet(byte, 4);
		apu->pulse_channel[0].envelope_divider = byte & 15;
	}
	else if (loc == 0x4002)
	{
		apu->pulse_channel[0].timer &= 0x700;
		apu->pulse_channel[0].timer |= byte;
		apu->pulse_channel[0].timer_target = apu->pulse_channel[0].timer;
	}
	else if (loc == 0x4003)
	{
		apu->pulse_channel[0].timer |= (byte << 8) & 0x700;
		apu->pulse_channel[0].len = lentable[(byte >> 3) & 0x1f];
		apu->pulse_channel[0].env_start = true;
		apu->pulse_channel[0].timer_target = apu->pulse_channel[0].timer;
	}
	else if (loc == 0x4004)
	{
		apu->pulse_channel[1].duty_cycle = (byte >> 6) & 3;
		apu->pulse_channel[1].length_counter_halt = utility::IsBitSet(byte, 5);
		apu->pulse_channel[1].c = utility::IsBitSet(byte, 4);
		apu->pulse_channel[1].envelope_divider = byte & 15;
	}
	else if (loc == 0x4006)
	{
		apu->pulse_channel[1].timer &= 0x700;
		apu->pulse_channel[1].timer |= byte;
		apu->pulse_channel[1].timer_target = apu->pulse_channel[0].timer;
	}
	else if (loc == 0x4007)
	{
		apu->pulse_channel[1].timer |= (byte << 8) & 0x700;
		apu->pulse_channel[1].len = lentable[(byte >> 3) & 0x1f];
		apu->pulse_channel[1].env_start = true;
		apu->pulse_channel[1].timer_target = apu->pulse_channel[0].timer;
	}
	else if (loc == 0x4015)
	{
		bool pulse1enable = utility::IsBitSet(byte, 0);
		bool pulse2enable = utility::IsBitSet(byte, 1);
		if (!pulse1enable)
		{
			apu->pulse_channel[0].enable = false;
			apu->pulse_channel[0].len = 0;
		}
		else
		{
			apu->pulse_channel[0].enable = true;
		}
		if (!pulse2enable)
		{
			apu->pulse_channel[1].enable = false;
			apu->pulse_channel[1].len = 0;
		}
		else
		{
			apu->pulse_channel[1].enable = true;
		}
	}
	else if (loc == 0x4017)
	{
		if (utility::IsBitSet(byte, 6))
		{
			apu->frame_counter.interrupt = true;
		}
		apu->frame_counter.mode = utility::IsBitSet(byte, 7);
	}

	cpu_data[loc] = byte;

}

uint8_t Memory::ReadCPUSafe(size_t loc)
{
	//mirroring
	if (loc >= 0x0800 && loc <= 0x1FFF)
	{
		loc &= 0x07FF;
	}
	else if (loc >= 0x2008 && loc <= 0x3FFF)
	{
		loc &= 0x2007;
	}

	return cpu_data[loc];
}

uint8_t Memory::ReadCPU(size_t loc)
{
	//mirroring
	if (loc >= 0x0800 && loc <= 0x1FFF)
	{
		loc &= 0x07FF;
	}
	else if (loc >= 0x2008 && loc <= 0x3FFF)
	{
		loc &= 0x2007;
	}

	/*if (loc == 0x2000 || loc == 0x2001 || loc == 0x2003 || loc == 0x2005 || loc == 0x2006 || loc == 0x4014)
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Memory::ReadCPU - Reading write only memory! " + utility::int_to_hex(loc));
		return 0;
	}*/

	if (loc == (size_t)ConstAddr::PPUDATA)
	{
		auto ppudata = &ppu_registers->ppudata;
		auto ppuaddr = &ppu_registers->ppuaddr;
		auto ppuctrl = &ppu_registers->ppuctrl;

		if (ppuaddr->GetAddr() >= 0x3F00)
		{
			cpu_data[loc] = ReadPPU(ppuaddr->GetAddr());
			ppudata->Set(ReadPPU(ppuaddr->GetAddr()-0x1000));
		}
		else
		{
			cpu_data[loc] = ppudata->Get();
			ppudata->Set(ReadPPU(ppuaddr->GetAddr()));
		}

		if (ppuctrl->IsBitSet(ControllerBits::VRAM_INC))
			ppuaddr->Add(32);
		else
			ppuaddr->Add(1);
	}
	else if (loc == (size_t)ConstAddr::PPUSTATUS)
	{
		auto ppuaddr = &ppu_registers->ppuaddr;
		auto ppuscroll = &ppu_registers->ppuscroll;
		auto ppustatus = &ppu_registers->ppustatus;
		ppu_registers->w = 0;
		cpu_data[loc] = ppustatus->Get();
		ppustatus->SetBit(StatusBits::VBLANK,false);
	}
	else if (loc == (size_t)ConstAddr::OAMDATA)
	{
		cpu_data[loc] = oam_data[ppu_registers->oamaddr];
	}
	else if (loc == 0x4016)
	{
		cpu_data[loc] = joypad[0].Read();
	}
	else if (loc == 0x4017)
	{
		cpu_data[loc] = joypad[1].Read();
	}
	else if (loc == 0x4015)
	{
		cpu_data[loc] = 0;
		if (apu->pulse_channel[0].len > 0)
		{
			cpu_data[loc] = cpu_data[loc] | 1;
		}
		if (apu->frame_counter.interrupt == false)
		{
			apu->frame_counter.interrupt = true;
			cpu_data[loc] = cpu_data[loc] | 1 << 7;
		}
	}

	return cpu_data[loc];
}

void Memory::WritePPU(size_t loc, uint8_t byte)
{
	//mirroring
	if (nametable_mirroring == 0)
	{
		if (loc >= 0x2800 && loc < 0x3000)
		{
			loc &= 0x27FF;
		}
	}
	else if (nametable_mirroring == 1)
	{
		if (loc >= 0x2400 && loc < 0x2800)
		{
			loc &= 0x23FF;
		}
		else if (loc >= 0x2C00 && loc < 0x3000)
		{
			loc &= 0x2BFF;
		}
	}
	if (loc >= 0x3000 && loc <= 0x3EFF)
	{
		loc &= 0x2EFF;
	}
	else if (loc >= 0x3f20 && loc <= 0x3FFF)
	{
		loc &= 0x3f1f;
	}
	else if (loc == 0x3f10 || loc == 0x3f14 || loc == 0x3f18 || loc == 0x3f1C)
	{
		loc -= 0x10;
	}
	else if (mapper.chr_ram && loc < 0x2000)
	{
		ppu_data[loc] = byte;
		BuildPixelValue((uint8_t)(loc / 0x1000), (uint8_t)((loc / 2) & 0xff));
		//logger::PrintLine(logger::LogType::WARNING, "Wrote to chr RAM");
	}

	ppu_data[loc] = byte;
}

uint8_t Memory::ReadPPU(size_t loc)
{
	//mirroring
	if (nametable_mirroring == 0)
	{
		if (loc >= 0x2800 && loc < 0x3000)
		{
			loc &= 0x27FF;
		}
	}
	else if (nametable_mirroring == 1)
	{
		if (loc >= 0x2400 && loc < 0x2800)
		{
			loc &= 0x23FF;
		}
		else if (loc >= 0x2C00 && loc < 0x3000)
		{
			loc &= 0x2BFF;
		}
	}
	if (loc >= 0x3000 && loc <= 0x3EFF)
	{
		loc &= 0x2EFF;
	}
	else if (loc >= 0x3f20 && loc <= 0x3FFF)
	{
		loc &= 0x3f1f;
	}
	else if (loc == 0x3f10 || loc == 0x3f14 || loc == 0x3f18 || loc == 0x3f1C)
	{
		loc -= 0x10;
	}

	return ppu_data[loc];
}

uint8_t Memory::ReadCHR(size_t loc)
{
	assert(loc < 0x2000);
	return ppu_data[loc];
}

bool Memory::LoadNES(NesData *nes_data)
{
	mapper.number = nes_data->header.mapper_num;

	if (utility::IsBitSet(nes_data->header.flags6, 0))
		nametable_mirroring = 0;
	else if(utility::IsBitSet(nes_data->header.flags6, 3))
		nametable_mirroring = 2;
	
	//this should probably be made with functions... this can get really messy with many mappers
	switch (mapper.number)
	{

	case 0:
	{
		mapper.name = "NROM";
		int start_prg_1 = 0x8000;
		int start_prg_2 = 0xC000;
		assert(nes_data->header.PRG_ROM_size <= 2);
		assert(nes_data->header.CHR_ROM_size == 1 || nes_data->header.CHR_ROM_size == 0);
		if (nes_data->header.CHR_ROM_size == 1)
		{
			char* dat_chr = nes_data->chr_rom.at(0).get();
			for (int i = 0; i < INES_CHR_BLOCK_SIZE; i++)
			{
				ppu_data[i] = dat_chr[i];
			}
			mapper.chr_ram = false;
		}
		else
		{
			mapper.chr_ram = true;
		}
		
		if (nes_data->header.PRG_ROM_size == 1)
		{
			char* dat = nes_data->prg_rom.at(0).get();
			for (int i = 0; i < INES_PRG_BLOCK_SIZE; i++)
			{
				cpu_data[start_prg_1 + i] = dat[i];
				cpu_data[start_prg_2 + i] = dat[i];
			}
		}
		else
		{
			char* dat = nes_data->prg_rom.at(0).get();
			char* dat2 = nes_data->prg_rom.at(1).get();
			for (int i = 0; i < INES_PRG_BLOCK_SIZE; i++)
			{
				cpu_data[start_prg_1 + i] = dat[i];
				cpu_data[start_prg_2 + i] = dat2[i];
			}
		}
		break;
	}
	default :
	{
		logger::PrintLine(logger::LogType::INTERNAL_ERROR, "Memory::LoadNES - Unsupported mapper");
		return false;
	}

	}

	int initial_pc = cpu_data[0xFFFD]*256+cpu_data[0xFFFC];
	logger::PrintLine(logger::LogType::INFO, "Mapper name: " + mapper.name + " Program start would be: " + utility::int_to_hex(initial_pc));

	return true;
}

void Memory::AttachStuff(PpuRegisters *ppu_registers, Joypad *joypad, Apu *apu)
{
	this->ppu_registers = ppu_registers;
	this->joypad = joypad;
	this->apu = apu;
}

void Memory::BuildPixelValues()
{
	uint32_t offset = 0;

	for (int bank = 0; bank < 2; bank++)
	{
		for (int index = 0; index < TILE_PER_BANK; index++)
		{
			if (bank == 1)
				offset = 0x1000;
			int counter = 0;
			for (int i = 0; i < TILE_HEIGHT; i++)
			{
				for (int j = 0; j < TILE_WIDTH; j++)
				{
					uint8_t data1 = ReadCHR(offset + BYTES_PER_TILE * index + i);
					uint8_t data2 = ReadCHR(offset + BYTES_PER_TILE * index + i + 8);
					int bit1 = (int)(data1 & (1 << (7 - j))) != 0;
					int bit2 = (int)(data2 & (1 << (7 - j))) != 0;
					uint8_t value = 1 * bit1 + 2 * bit2;
					pixel_values[bank][index*PIXEL_PER_TILE + counter] = value;
					counter++;

				}
			}
		}
	}
}

void Memory::BuildPixelValue(uint8_t bank, uint8_t index)
{
	uint32_t offset = 0;
	if (bank == 1)
		offset = 0x1000;
	int counter = 0;
	for (int i = 0; i < TILE_HEIGHT; i++)
	{
		for (int j = 0; j < TILE_WIDTH; j++)
		{
			uint8_t data1 = ReadCHR(offset + BYTES_PER_TILE * index + i);
			uint8_t data2 = ReadCHR(offset + BYTES_PER_TILE * index + i + 8);
			int bit1 = (int)(data1 & (1 << (7 - j))) != 0;
			int bit2 = (int)(data2 & (1 << (7 - j))) != 0;
			uint8_t value = 1 * bit1 + 2 * bit2;
			pixel_values[bank][index*PIXEL_PER_TILE + counter] = value;
			counter++;

		}
	}
}