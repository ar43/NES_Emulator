#pragma once
#include "../cpu/addressingmode.h"
#include <stdint.h>
#include <string>

struct Instruction
{
	Instruction(uint8_t opcode, std::string name, AddressingMode mode, uint8_t cycles)
	{
		this->opcode = opcode;
		this->name = name;
		this->mode = mode;
		this->cycles = cycles;
	}
	uint8_t opcode;
	std::string name;
	AddressingMode mode;
	uint8_t cycles;
};