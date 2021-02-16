#include "cpu.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include <iostream>


Cpu::Cpu()
{
	std::fill(std::begin(flags), std::end(flags), false);

	

}

void Cpu::AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t cycles)
{
	auto it = instruction_set.find(opcode);
	if (it != instruction_set.end())
	{
		logger::PrintLine(logger::LogType::ERROR, "Cpu::AddInstruction - trying to add an instruction with same opcode (name: " + name + " | op: " + utility::int_to_hex(opcode) + ")");
		return;
	}

	auto *instruction = new Instruction(opcode, name, mode, cycles);
	instruction_set[opcode] = instruction;
}

Instruction* Cpu::GetInstruction(uint8_t opcode)
{
	auto it = instruction_set.find(opcode);
	if (it != instruction_set.end())
	{
		return instruction_set[opcode];
	}
	else
	{
		logger::PrintLine(logger::LogType::ERROR, "Cpu::GetInstruction - unknown instruction (op: " + utility::int_to_hex(opcode) + ")");
		return nullptr;
	}
}

void Cpu::DumpRegisters()
{
	logger::PrintLine(logger::LogType::INFO, "Dumping registers:");
	for (size_t i = 0; i < (size_t)RegId::NUM_REGISTERS;i++)
	{
		std::string name = Register::to_string((RegId)i);
		std::cout << name << " = " << std::to_string(registers[i].get()) << std::endl;
	}
	logger::PrintLine(logger::LogType::INFO, "Dumping flags:");
	for (size_t i = 0; i < (size_t)flags::Flags::NUM_FLAGS;i++)
	{
		std::string name = flags::to_string((flags::Flags)i);
		std::cout << name << " = " << std::to_string(flags[i]) << std::endl;
	}
}