#pragma once
#include <array>
#include <string>
#include <memory>

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
	}
	static const size_t CPU_MEM_SIZE = 0x10000;
	static const size_t PPU_MEM_SIZE = 0x4000;

	void WriteCPU(size_t loc, uint8_t byte);
	uint8_t ReadCPU(size_t loc);

	void WritePPU(size_t loc, uint8_t byte);
	uint8_t ReadPPU(size_t loc);

	bool LoadNES(NesData *nes_data);

private:
	std::array<uint8_t, Memory::CPU_MEM_SIZE> cpu_data;
	std::array<uint8_t, Memory::PPU_MEM_SIZE> ppu_data;
	Mapper mapper;
};