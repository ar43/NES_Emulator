#pragma once
#include <array>
#include <string>
#include <memory>
#include "../ppu/ppuregisters.h"
#include "../misc/constants.h"

struct NesData;
class Joypad;
class Apu;
class Cpu;
class Ppu;
class Mapper;

class Bus
{
public:
	void WriteCPU(size_t loc, uint8_t byte); //CPU refers to address space
	uint8_t ReadCPU(size_t loc);
	uint8_t ReadCPUSafe(size_t loc);

	void WritePPU(size_t loc, uint8_t byte); //PPU refers to address space
	uint8_t ReadPPU(size_t loc);

	void AttachComponents(Cpu *cpu, Joypad *joypad, Apu *apu, Ppu *ppu);
	void AttachMapper(Mapper* mapper);

	bool nmi_pending = false;
	bool irq_pending = false;
	bool *dmcirq_pending = nullptr;
	bool add_dma_cycles = false;

	//int nametable_mirroring = 1; //0 vertical mirroring, 1 horizontal mirroring, 2 other //this is supposed to be in the mapper

private:
	Joypad* joypad;
	Apu* apu;
	Ppu* ppu;
	Cpu* cpu;
	Mapper* mapper; //careful!!! make sure to replace this whenever switching a mapper
};