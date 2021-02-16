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
	
	int GetFlagsValue();

	std::map<uint8_t, Instruction*> instruction_set;

	void AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t bytes, uint8_t cycles, std::function<void(Cpu*,Memory*,int)> f, bool extra_cycle = false);
	Instruction* GetInstruction(uint8_t opcode);

	void LoadInstructionSet();
	void LoadLSOperations();
	void LoadRTOperations();
	void LoadStackOperations();
	void LoadLogicalOperations();
	void LoadArithmeticOperations();
	void LoadIDOperations();
	void LoadShiftsOperations();
	void LoadJCOperations();
	void LoadBranchesOperations();
	void LoadStatusFlagOperations();
	void LoadSystemFunctionOperations();

	void InitRegisters();

	uint8_t Fetch(Memory *mem);
	void ExecuteInstruction(Memory* mem);
	int ResolveAddressing(Memory* mem, AddressingMode mode, bool extra_cycle);

	uint64_t cycle_counter = 0;
	void AddCycles(uint8_t num);
};