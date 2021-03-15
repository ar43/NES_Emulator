#pragma once
#include "register.h"
#include "../../machine/misc/instruction.h"
#include <array>
#include <map>

constexpr size_t STACK_OFFSET = 0x100;

class Bus;

class Cpu
{
public:
	Cpu();

	std::string RegistersToString();
	std::string PPUCounterToString();
	std::string CYCToString();
	std::string GetFetchBuffer();

	Register* registers[static_cast<size_t>(RegId::NUM_REGISTERS)];
	void AddCycles(uint32_t num);
	uint64_t GetCycles();
	

	void ExecuteInstruction(Bus* bus);
	void HandleReset(Bus* bus, int reset);
	void HandleNMI(Bus* bus);
	void HandleIRQ(Bus* bus);

	void WriteRam(size_t loc, uint8_t byte);
	uint8_t ReadRam(size_t loc);

	std::string output_string;
	
private:

	Instruction* instruction_set[256];

	void AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t bytes, uint8_t cycles, void (*f)(Cpu*, Bus*, int, AddressingMode mode), bool extra_cycle = false);
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

	uint8_t Fetch(Bus *bus);
	std::string fetch_buffer;
	
	int ResolveAddressing(Bus *bus, Instruction* ins);

	uint64_t cycle_counter = 0;
	bool add_extra_cycle = false;

	uint8_t RAM[0x800]; //2KB internal ram
	
};