#include "bus.h"
#include "../cpu/cpu.h"
#include "../input/joypad.h"
#include "../apu/apu.h"
#include "../ppu/ppu.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include "../mapper/mapper.h"
#include "../misc/constants.h"

void Bus::AttachComponents(Cpu* cpu, Joypad *joypad, Apu *apu, Ppu *ppu)
{
	this->cpu = cpu;
	this->joypad = joypad;
	this->apu = apu;
	this->ppu = ppu;
	this->dmcirq_pending = &apu->dmc_channel.dmcirq_pending;
}

void Bus::AttachMapper(Mapper* mapper)
{
	this->mapper = mapper;
}

void Bus::ChangeNametableMirroring(int old)
{
	if (old == 3 && mapper->nametable_mirroring == 2)
	{
		//logger::PrintLine(logger::LogType::DEBUG, "Nametable mirroring switched from 3 to 2");
		memcpy(ppu->data + 0x2400, ppu->data + 0x2800, 0x400);
	}
	else if (old == 2 && mapper->nametable_mirroring == 3)
	{
		//logger::PrintLine(logger::LogType::DEBUG, "Nametable mirroring switched from 2 to 3");
		memcpy(ppu->data + 0x2800, ppu->data + 0x2400, 0x400);
	}
}

void Bus::Reset()
{
	mapper->ClearRegisters();
}

void Bus::WriteCPU(size_t loc, uint8_t byte)
{
	if (loc <= 0x1FFF) //internal CPU RAM area
	{
		cpu->WriteRam(loc, byte);
	}
	else if (loc <= 0x3FFF || loc == 0x4014) //PPU registers
	{
		ppu->WriteRegisters(this, loc, byte);
	}
	else if (loc == 0x4016) //joypad
	{
		joypad[0].Write(byte);
		joypad[1].Write(byte);
	}
	else if (loc <= 0x4017) //apu registers
	{
		apu->WriteRegisters(loc, byte);
	}
	else if (loc <= 0xFFFF) //mapper stuff here (cartridge)
	{
		const int old_nametable_mirroring = mapper->nametable_mirroring;
		mapper->WritePRG(loc, byte);
		ChangeNametableMirroring(old_nametable_mirroring);
	}
	else
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Bus::WriteCPU out of bounds");
	}

}

uint8_t Bus::ReadCPU(size_t loc)
{
	if (loc <= 0x1FFF) //internal CPU RAM area
	{
		return cpu->ReadRam(loc);
	}
	else if (loc <= 0x3FFF || loc == 0x4014) //PPU registers
	{
		return ppu->ReadRegisters(this, loc);
	}
	else if (loc == 0x4016)
	{
		return joypad[0].Read();
	}
	else if (loc == 0x4017)
	{
		return joypad[1].Read();
	}
	else if (loc == 0x4015)
	{
		return apu->ReadStatus();
	}
	else if (loc <= 0xFFFF)
	{
		//mapper stuff here
		return mapper->ReadPRG(loc);
	}
	else
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Bus::ReadCPU - cannot read addr " + utility::int_to_hex(loc));
		return 0;
	}
}

uint8_t Bus::ReadCPUSafe(size_t loc)
{
	if (loc <= 0x1FFF) //internal CPU RAM area
	{
		return cpu->ReadRam(loc);
	}
	else if (loc <= 0xFFFF)
	{
		//mapper stuff here
		return mapper->ReadPRG(loc);
	}
	else
	{
		//logger::PrintLine(logger::LogType::WARNING, "Bus::ReadCPUSafe - cannot read addr " + utility::int_to_hex(loc));
		return 0;
	}
}

void Bus::WritePPU(size_t loc, uint8_t byte)
{
	assert(loc < PPU_MEM_SIZE);
	if (mapper->nametable_mirroring == 2)
	{
		if (loc >= 0x2800 && loc < 0x3000)
		{
			loc &= 0x27FF;
		}
	}
	else if (mapper->nametable_mirroring == 3)
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
	else
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Unsupported nametable mirroring");
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
	bool status = false;
	status = mapper->WritePPU(loc, byte);

	if (status)
	{
		if (mapper->use_chr_ram && loc < 0x2000)
		{
			//ppu->display.BuildPixelValue(this, (uint8_t)(loc / 0x1000), (uint8_t)((loc / 2) & 0xff));
			rebuild_pixels = true;
		}
		return;
	}
	
	

	ppu->data[loc] = byte;
}

uint8_t Bus::ReadPPU(size_t loc)
{
	assert(loc < PPU_MEM_SIZE);
	if (mapper->nametable_mirroring == 2)
	{
		if (loc >= 0x2800 && loc < 0x3000)
		{
			loc &= 0x27FF;
		}
	}
	else if (mapper->nametable_mirroring == 3)
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
	else
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Unsupported nametable mirroring");
	}


	if (loc >= 0x3000 && loc <= 0x3EFF)
	{
		loc &= 0x2EFF;
	}
	else if (loc >= 0x3f20 && loc <= 0x3FFF)
	{
		loc &= 0x3f1f;
	}
	if (loc == 0x3f10 || loc == 0x3f14 || loc == 0x3f18 || loc == 0x3f1C)
	{
		loc -= 0x10;
	}

	int ret = mapper->ReadPPU(loc);
	if (ret >= 0)
		return ret;
	else
		return ppu->data[loc];
}
