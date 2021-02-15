#pragma once
#include <array>

class Memory
{
public:
	Memory() 
	{
		std::fill(std::begin(data), std::end(data), 0);
	}
	static const size_t MEM_SIZE = 0x10000;

	void Write(size_t loc, uint8_t byte);
	uint8_t Read(size_t loc);
private:
	std::array<uint8_t, Memory::MEM_SIZE> data;
};