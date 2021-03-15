#include "instruction.h"

Instruction::Instruction(uint8_t opcode, std::string name, AddressingMode mode,uint8_t bytes, uint8_t cycles, void (*f)(Cpu*, Bus*, int, AddressingMode mode), bool extra_cycle)
{
	this->opcode = opcode;
	this->name = name;
	this->mode = mode;
	this->bytes = bytes;
	this->cycles = cycles;
	this->func = f;
	this->extra_cycle = extra_cycle;
}