#pragma once
#include <array>
#include <string>
#include <memory>
#include "../ppu/ppuregisters.h"

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

struct Mapper
{
	std::string name;
	int number;
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

	bool LoadNES(NesData *nes_data);

	void AttachPPURegisters(PpuRegisters* ppu_registers);
	PpuRegisters *ppu_registers;
	
	bool add_dma_cycles = false;
	bool trigger_nmi_interrupt = false;
	uint8_t chr_rom[CHR_ROM_SIZE];
private:
	uint8_t cpu_data[CPU_MEM_SIZE];
	uint8_t ppu_data[PPU_MEM_SIZE];
	uint8_t oam_data[OAM_MEM_SIZE];
	
	

	Mapper mapper;
};