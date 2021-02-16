#pragma once
#include "register.h"
#include "flags.h"
#include "../../machine/misc/instruction.h"
#include <array>
#include <map>

class Cpu
{
public:
	Cpu();
	void DumpRegisters();
	
private:
	std::array<Register, static_cast<size_t>(RegId::NUM_REGISTERS)> registers;
	std::array<bool, static_cast<size_t>(flags::Flags::NUM_FLAGS)> flags;
	std::map<uint8_t, Instruction*> instruction_set;

	void AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t cycles);
	Instruction* GetInstruction(uint8_t opcode);
};