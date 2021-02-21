#pragma once
#include "register.h"
#include "../../machine/misc/instruction.h"
#include <array>
#include <map>

constexpr size_t STACK_OFFSET = 0x100;

class Cpu
{
public:
	Cpu();

	std::string RegistersToString();
	std::string PPUCounterToString();
	std::string CYCToString();
	std::string GetFetchBuffer();

	void RunTest(Memory *mem, int count);

	std::array<Register*, static_cast<size_t>(RegId::NUM_REGISTERS)> registers;
	
	void AddCycles(uint32_t num);
	uint64_t GetCycles();
	

	void ExecuteInstruction(Memory* mem);
	void PollReset(Memory* mem);
	void PollNMI(Memory* mem);
	
private:

	std::map<uint8_t, Instruction*> instruction_set;

	void AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t bytes, uint8_t cycles, std::function<void(Cpu*,Memory*,int, AddressingMode mode)> f, bool extra_cycle = false);
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
	std::string fetch_buffer;
	
	int ResolveAddressing(Memory* mem, Instruction* ins, std::string & out);

	uint64_t cycle_counter = 0;
	bool add_extra_cycle = false;
	bool reset = true;
	
};