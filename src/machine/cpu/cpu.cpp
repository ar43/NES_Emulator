#include "cpu.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include "../memory/memory.h"
#include "opcodes.h"
#include <iostream>


Cpu::Cpu()
{
	std::fill(std::begin(flags), std::end(flags), false);
	InitRegisters();
	BuildInstructionSet();
}

void Cpu::RunTest(Memory* mem)
{
	auto ins = GetInstruction(0xA9);
	ins->func(this, mem, 15);
	DumpRegisters();
}

void Cpu::BuildInstructionSet()
{
	AddInstruction("LDA", 0xA9, AddressingMode::IMMEDIATE, 2, &opcodes::LDA);
}

void Cpu::InitRegisters()
{
	for (size_t i = 0; i < registers.size();i++)
	{
		if ((RegId)i == RegId::PC)
		{
			registers[i] = new Register(RegType::BIT16);
		}
		else
		{
			registers[i] = new Register(RegType::BIT8);
		}
	}
}

void Cpu::AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t cycles, std::function<void(Cpu*,Memory*,int)> f)
{
	auto it = instruction_set.find(opcode);
	if (it != instruction_set.end())
	{
		logger::PrintLine(logger::LogType::ERROR, "Cpu::AddInstruction - trying to add an instruction with same opcode (name: " + name + " | op: " + utility::int_to_hex(opcode) + ")");
		return;
	}

	auto *instruction = new Instruction(opcode, name, mode, cycles, f);
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
	for (size_t i = 0; i < registers.size();i++)
	{
		std::string name = Register::to_string((RegId)i);
		std::cout << name << " = " << utility::int_to_hex(registers[i]->get()) << " (" << std::to_string(registers[i]->get()) << ")"<< std::endl;
	}
	logger::PrintLine(logger::LogType::INFO, "Dumping flags:");
	for (size_t i = 0; i < flags.size();i++)
	{
		std::string name = flags::to_string((flags::Flags)i);
		std::cout << name << " = " << std::to_string(flags[i]) << std::endl;
	}
}