#include "cpu.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include "../memory/memory.h"
#include <iostream>


Cpu::Cpu()
{
	std::fill(std::begin(flags), std::end(flags), false);
	flags[(size_t)flags::Flags::B] = true;
	flags[(size_t)flags::Flags::I] = true;
	logger::PrintLine(logger::LogType::INFO, "P=" + utility::int_to_hex(GetFlagsValue()));
	InitRegisters();
	LoadInstructionSet();
}

void Cpu::RunTest(Memory* mem)
{
	auto ins = GetInstruction(0xA9);
	ins->func(this, mem, 0);
	DumpRegisters();

	int a = 0;
	uint8_t ms = 0x12;
	uint8_t ls = 0x34;
	a = (ms << 8) | ls;
	std::cout << utility::int_to_hex(a);


}

uint8_t Cpu::Fetch(Memory* mem)
{
	uint8_t ret = mem->Read(registers[(size_t)RegId::PC]->get());
	registers[(size_t)RegId::PC]->increment();
	return ret;
}

int Cpu::ResolveAddressing(Memory* mem, AddressingMode mode, bool extra_cycle)
{
	switch (mode)
	{

	case AddressingMode::IMPLICIT:
	{
		return 0;
	}
	case AddressingMode::ACCUMULATOR:
	{
		//special case, beware!
		//todo: set some special flag on cpu to perform stuff directly on A
		//and make sure that the relevant functions in "opcodes" check that
		logger::PrintLine(logger::LogType::ERROR, "Unsupported addressing - ACCUMULATOR");
		return 0;
	}
	case AddressingMode::IMMEDIATE:
	{
		auto fetched = Fetch(mem);
		return fetched;
	}
	case AddressingMode::ZERO_PAGE:
	{
		auto fetched = Fetch(mem);
		return fetched;
	}
	case AddressingMode::ZERO_PAGE_X:
	{
		uint8_t fetched = Fetch(mem);
		int x = registers[(size_t)RegId::X]->get();
		int addr = fetched + x;
		if (addr > 0xFF)
			addr -= 0xFF;
		return addr;
	}
	case AddressingMode::ZERO_PAGE_Y:
	{
		uint8_t fetched = Fetch(mem);
		int y = registers[(size_t)RegId::Y]->get();
		int addr = fetched + y;
		if (addr > 0xFF)
			addr -= 0xFF;
		return addr;
	}
	case AddressingMode::RELATIVE:
	{
		uint8_t fetched = Fetch(mem);
		int ret = (int8_t)fetched;
		return ret;
	}
	case AddressingMode::ABSOLUTE:
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		int ret = (ms << 8) | ls;
		return ret;
	}
	case AddressingMode::ABSOLUTE_X: //extra cycle support
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		int first = (ms << 8) | ls;

		int x = registers[(size_t)RegId::X]->get();
		int second = first + x;

		int first_page = first % 256;
		int second_page = second % 256;

		if (extra_cycle && first_page != second_page)
			AddCycles(1);

		return second;
	}
	case AddressingMode::ABSOLUTE_Y: //extra cycle support
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		int first = (ms << 8) | ls;

		int y = registers[(size_t)RegId::Y]->get();
		int second = first + y;

		int first_page = first % 256;
		int second_page = second % 256;

		if (extra_cycle && first_page != second_page)
			AddCycles(1);

		return second;
	}
	case AddressingMode::INDIRECT:
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		size_t addr = (ms << 8) | ls;

		assert(addr < 0xFFFF);
		uint8_t ls_lookup = mem->Read(addr);
		uint8_t ms_lookup = mem->Read(addr+1);

		int ret = (ms_lookup << 8) | ls_lookup;

		return ret;
	}
	case AddressingMode::INDEXED_INDIRECT:
	{
		uint8_t fetched = Fetch(mem);
		int x = registers[(size_t)RegId::X]->get();
		int addr = fetched + x;
		if (addr > 0xFF)
			addr -= 0xFF;

		assert(fetched < 0xFF);
		uint8_t ls_lookup = mem->Read(addr);
		uint8_t ms_lookup = mem->Read(addr+1);

		int ret = (ms_lookup << 8) | ls_lookup;
		return ret;
	}
	case AddressingMode::INDIRECT_INDEXED: //extra cycle support
	{
		uint8_t fetched = Fetch(mem);
		assert(fetched < 0xFF);
		uint8_t ls_lookup = mem->Read(fetched);
		uint8_t ms_lookup = mem->Read(fetched+1);

		int first = (ms_lookup << 8) | ls_lookup;
		int y = registers[(size_t)RegId::Y]->get();
		int second = first + y;

		int first_page = first % 256;
		int second_page = second % 256;

		if (extra_cycle && first_page != second_page)
			AddCycles(1);

		return second;
	}
	default:
	{
		logger::PrintLine(logger::LogType::ERROR, "Unknown addressing " + std::to_string((size_t)mode));
		return 0;
	}

	}
}

void Cpu::ExecuteInstruction(Memory *mem)
{
	uint8_t opcode = Fetch(mem);
	auto instruction = GetInstruction(opcode);
	int value = ResolveAddressing(mem, instruction->mode, instruction->extra_cycle);
	instruction->func(this, mem, value);
	AddCycles(instruction->cycles);
}

void Cpu::AddCycles(uint8_t num)
{
	cycle_counter += num;
}

int Cpu::GetFlagsValue()
{
	return (int)flags[0] | (int)flags[1] << 1 | (int)flags[2] << 2 | (int)flags[3] << 3 | (int)flags[4] << 5 | (int)flags[5] << 6 | (int)flags[6] << 7;
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

	registers[(size_t)RegId::SP]->set(0xFD);
}

void Cpu::AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t bytes, uint8_t cycles, std::function<void(Cpu*,Memory*,int)> f, bool extra_cycle)
{
	auto it = instruction_set.find(opcode);
	if (it != instruction_set.end())
	{
		logger::PrintLine(logger::LogType::ERROR, "Cpu::AddInstruction - trying to add an instruction with same opcode (name: " + name + " | op: " + utility::int_to_hex(opcode) + ")");
		return;
	}

	auto *instruction = new Instruction(opcode, name, mode, bytes, cycles, f, extra_cycle);
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