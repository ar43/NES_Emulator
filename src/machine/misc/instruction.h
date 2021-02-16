#pragma once
#include "../cpu/addressingmode.h"
#include <stdint.h>
#include <string>
#include <functional>

class Cpu;
class Memory;

struct Instruction
{
	Instruction(uint8_t opcode, std::string name, AddressingMode mode, uint8_t cycles, std::function<void(Cpu*, Memory*, int)> f);
	uint8_t opcode;
	std::string name;
	AddressingMode mode;
	uint8_t cycles;
	std::function<void(Cpu*,Memory*,int)> func;
};