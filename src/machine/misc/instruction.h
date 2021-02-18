#pragma once
#include "../cpu/addressingmode.h"
#include <stdint.h>
#include <string>
#include <functional>

class Cpu;
class Memory;

struct Instruction
{
	Instruction(uint8_t opcode, std::string name, AddressingMode mode, uint8_t bytes, uint8_t cycles, std::function<void(Cpu*, Memory*, int, AddressingMode mode)> f, bool extra_cycle);
	uint8_t opcode;
	std::string name;
	AddressingMode mode;
	uint8_t bytes;
	uint8_t cycles;
	bool extra_cycle;
	std::function<void(Cpu*,Memory*,int, AddressingMode mode)> func;
};