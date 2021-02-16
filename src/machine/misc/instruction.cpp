#include "instruction.h"

Instruction::Instruction(uint8_t opcode, std::string name, AddressingMode mode, uint8_t cycles, std::function<void(Cpu*,Memory*,int)> f)
{
	this->opcode = opcode;
	this->name = name;
	this->mode = mode;
	this->cycles = cycles;
	this->func = f;
}