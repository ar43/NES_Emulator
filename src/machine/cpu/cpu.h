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
	void RunTest(Memory *mem);

	std::array<Register*, static_cast<size_t>(RegId::NUM_REGISTERS)> registers;
	std::array<bool, static_cast<size_t>(flags::Flags::NUM_FLAGS)> flags;
	
private:
	
	std::map<uint8_t, Instruction*> instruction_set;

	void AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t cycles, std::function<void(Cpu*,Memory*,int)> f);
	Instruction* GetInstruction(uint8_t opcode);
	void BuildInstructionSet();

	void InitRegisters();

	uint64_t cycleCounter = 0;
};