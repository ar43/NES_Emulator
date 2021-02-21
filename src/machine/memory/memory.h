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
		std::fill(std::begin(cpu_data), std::end(cpu_data), 0);
		std::fill(std::begin(ppu_data), std::end(ppu_data), 0);
	}
	static const size_t CPU_MEM_SIZE = 0x10000;
	static const size_t PPU_MEM_SIZE = 0x4000;
	static const size_t OAM_MEM_SIZE = 0x100;

	void WriteCPU(size_t loc, uint8_t byte);
	uint8_t ReadCPU(size_t loc);

	void WritePPU(size_t loc, uint8_t byte);
	uint8_t ReadPPU(size_t loc);

	bool LoadNES(NesData *nes_data);

	void AttachPPURegisters(PpuRegisters* ppu_registers);
	
	bool add_dma_cycles = false;
	bool trigger_nmi_interrupt = false;

private:
	std::array<uint8_t, Memory::CPU_MEM_SIZE> cpu_data;
	std::array<uint8_t, Memory::PPU_MEM_SIZE> ppu_data;
	std::array<uint8_t, Memory::OAM_MEM_SIZE> oam_data;
	
	PpuRegisters *ppu_registers;

	Mapper mapper;
};