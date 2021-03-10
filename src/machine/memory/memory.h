#pragma once
#include <array>
#include <string>
#include <memory>
#include "../ppu/ppuregisters.h"
#include "../misc/constants.h"

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

struct NesData;
class Joypad;
class Apu;

struct Mapper
{
	std::string name;
	int number = -1;
	bool chr_ram = false;
};

class Memory
{
public:
	Memory() 
	{
		memset(cpu_data, 0, CPU_MEM_SIZE * sizeof(uint8_t));
		memset(ppu_data, 0, PPU_MEM_SIZE * sizeof(uint8_t));
	}
	static const size_t CPU_MEM_SIZE = 0x10000;
	static const size_t PPU_MEM_SIZE = 0x4000;
	static const size_t OAM_MEM_SIZE = 0x100;
	static const size_t CHR_ROM_SIZE = 0x2000;

	void WriteCPU(size_t loc, uint8_t byte);
	uint8_t ReadCPU(size_t loc);
	uint8_t ReadCPUSafe(size_t loc);

	void WritePPU(size_t loc, uint8_t byte);
	uint8_t ReadPPU(size_t loc);
	uint8_t ReadCHR(size_t loc);

	bool LoadNES(NesData *nes_data);

	void AttachStuff(PpuRegisters* ppu_registers, Joypad *joypad, Apu *apu);
	PpuRegisters *ppu_registers;

	void BuildPixelValues();
	void BuildPixelValue(uint8_t bank, uint8_t index);
	
	bool add_dma_cycles = false;
	bool trigger_nmi_interrupt = false;
	bool trigger_irq_interrupt = false;
	uint8_t oam_data[OAM_MEM_SIZE];
	uint8_t pixel_values[2][256 * 64];
	Joypad* joypad;
	Apu* apu;

	int nametable_mirroring = 1; //0 vertical mirroring, 1 horizontal mirroring, 2 other

private:
	uint8_t cpu_data[CPU_MEM_SIZE];
	uint8_t ppu_data[PPU_MEM_SIZE];
	

	Mapper mapper;
};