#pragma once
#include "../cpu/addressingmode.h"
#include <stdint.h>
#include <string>
#include <functional>

class Cpu;
class Memory;

struct Instruction
{
	Instruction(uint8_t opcode, std::string name, AddressingMode mode, uint8_t bytes, uint8_t cycles, void (*f)(Cpu*, Memory*, int, AddressingMode mode), bool extra_cycle);
	uint8_t opcode;
	std::string name;
	AddressingMode mode;
	uint8_t bytes;
	uint8_t cycles;
	bool extra_cycle;
	void (*func)(Cpu*, Memory*, int, AddressingMode mode);
};