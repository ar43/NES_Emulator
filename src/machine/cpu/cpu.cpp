#include "cpu.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include "../memory/memory.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Cpu::Cpu()
{
	std::fill(std::begin(instruction_set), std::end(instruction_set), nullptr);
	InitRegisters();
	LoadInstructionSet();
	fetch_buffer = "";
	add_extra_cycle = false;
}

void Cpu::RunTest(Memory* mem, int count)
{
	logger::PrintLine(logger::LogType::INFO, "Running CPU Test");

	//int initial_pc = mem->ReadCPU(0xFFFD)*256 + mem->ReadCPU(0xFFFC); //normally we jump to this
	//registers[(size_t)RegId::PC]->set(initial_pc);

	registers[(size_t)RegId::PC]->set(0xC000); //need to force nestest to load at this location or it wont work (not without PPU anyways)

	for (int i = 0; i < count; i++)
	{
		ExecuteInstruction(mem);
	}
	logger::WriteTestToFile();
	logger::PrintLine(logger::LogType::INFO, "Finished CPU Test");
}

uint8_t Cpu::Fetch(Memory* mem)
{
	uint8_t ret = mem->ReadCPUSafe(registers[(size_t)RegId::PC]->get());
	registers[(size_t)RegId::PC]->increment();

	if (logger::CPU_TEST_MODE)
	{
		fetch_buffer += utility::int_to_hex(ret) + " ";
	}

	return ret;
}

int Cpu::ResolveAddressing(Memory* mem, Instruction* ins)
{
	switch (ins->mode)
	{

	case AddressingMode::IMPLICIT:
	{
		return 0;
	}
	case AddressingMode::ACCUMULATOR:
	{
		//special case, beware!
		//have to take care of this on opcode level
		if(logger::CPU_TEST_MODE)
			output_string = "A";
		return 0;
	}
	case AddressingMode::IMMEDIATE:
	{
		auto fetched = Fetch(mem);
		if(logger::CPU_TEST_MODE)
			output_string = "#$" + utility::int_to_hex(fetched);
		return fetched;
	}
	case AddressingMode::ZERO_PAGE:
	{
		auto fetched = Fetch(mem);
		if(logger::CPU_TEST_MODE)
			output_string = "$" + utility::int_to_hex(fetched) + " = " + utility::int_to_hex(mem->ReadCPUSafe(fetched));
		return fetched;
	}
	case AddressingMode::ZERO_PAGE_X:
	{
		uint8_t fetched = Fetch(mem);
		int x = registers[(size_t)RegId::X]->get();
		int addr = fetched + x;
		addr = addr % 0x100;
		if(logger::CPU_TEST_MODE)
			output_string = "$" + utility::int_to_hex(fetched) + ",X @ " + utility::int_to_hex(addr) + " = " + utility::int_to_hex(mem->ReadCPUSafe(addr));
		return addr;
	}
	case AddressingMode::ZERO_PAGE_Y:
	{
		uint8_t fetched = Fetch(mem);
		int y = registers[(size_t)RegId::Y]->get();
		int addr = fetched + y;
		addr = addr % 0x100;
		if(logger::CPU_TEST_MODE)
			output_string = "$" + utility::int_to_hex(fetched) + ",Y @ " + utility::int_to_hex(addr) + " = " + utility::int_to_hex(mem->ReadCPUSafe(addr));
		return addr;
	}
	case AddressingMode::RELATIVE:
	{
		uint8_t fetched = Fetch(mem);
		int ret = (int8_t)fetched;
		if(logger::CPU_TEST_MODE)
			output_string = "$" + utility::int_to_hex(ret + registers[(size_t)RegId::PC]->get());
		return ret;
	}
	case AddressingMode::ABSOLUTE:
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		int ret = (ms << 8) | ls;

		if(logger::CPU_TEST_MODE && ins->name.compare("JMP") != 0 && ins->name.compare("JSR") != 0)
			output_string = "$" + utility::int_to_hex(ret) + " = " + utility::int_to_hex(mem->ReadCPUSafe(ret));
		else if(logger::CPU_TEST_MODE)
			output_string = "$" + utility::int_to_hex(ret);

		return ret;
	}
	case AddressingMode::ABSOLUTE_X: //extra cycle support
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		int first = (ms << 8) | ls;

		int x = registers[(size_t)RegId::X]->get();
		int second = (first + x) % Memory::CPU_MEM_SIZE;

		int first_page = first / 256;
		int second_page = second / 256;

		if (ins->extra_cycle && first_page != second_page)
			add_extra_cycle = true;

		if(logger::CPU_TEST_MODE)
			output_string = "$" + utility::int_to_hex(first) + ",X @ " + utility::int_to_hex(second) + " = " + utility::int_to_hex(mem->ReadCPUSafe(second));
		return second;
	}
	case AddressingMode::ABSOLUTE_Y: //extra cycle support
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		int first = (ms << 8) | ls;

		int y = registers[(size_t)RegId::Y]->get();
		int second = (first + y) % Memory::CPU_MEM_SIZE;

		int first_page = first / 256;
		int second_page = second / 256;

		if (ins->extra_cycle && first_page != second_page)
			add_extra_cycle = true;

		if(logger::CPU_TEST_MODE)
			output_string = "$" + utility::int_to_hex(first) + ",Y @ " + utility::int_to_hex(second) + " = " + utility::int_to_hex(mem->ReadCPUSafe(second));
		return second;
	}
	case AddressingMode::INDIRECT:
	{
		uint8_t ls = Fetch(mem);
		uint8_t ms = Fetch(mem);
		size_t addr = (ms << 8) | ls;

		assert(addr < 0xFFFF);
		uint8_t ls_lookup = mem->ReadCPUSafe(addr);
		int old_addr = addr;
		addr = addr + 1;
		if (addr % 0x100 == 0)
			addr -= 0x100;
		uint8_t ms_lookup = mem->ReadCPUSafe(addr);

		int ret = (ms_lookup << 8) | ls_lookup;

		if(logger::CPU_TEST_MODE)
			output_string = "($" + utility::int_to_hex(old_addr) + ") = " + utility::int_to_hex(ret);

		return ret;
	}
	case AddressingMode::INDEXED_INDIRECT:
	{
		uint8_t fetched = Fetch(mem);
		int x = registers[(size_t)RegId::X]->get();
		int addr = fetched + x;
		if (addr > 0xFF)
			addr = addr % 0x100;

		uint8_t ls_lookup = mem->ReadCPUSafe(addr);
		int old_addr = addr;
		addr = (addr + 1) % 0x100;
		uint8_t ms_lookup = mem->ReadCPUSafe(addr);

		int ret = (ms_lookup << 8) | ls_lookup;

		if(logger::CPU_TEST_MODE)
			output_string = "($" + utility::int_to_hex(fetched) + ",X) @ " + utility::int_to_hex(old_addr) + " = " + utility::int_to_hex(ret) + " = " + utility::int_to_hex(mem->ReadCPUSafe(ret));

		return ret;
	}
	case AddressingMode::INDIRECT_INDEXED: //extra cycle support
	{
		uint8_t fetched = Fetch(mem);
		int addr = fetched;
		uint8_t ls_lookup = mem->ReadCPUSafe(addr);
		addr= (addr + 1) % 0x100;
		uint8_t ms_lookup = mem->ReadCPUSafe(addr);

		int first = (ms_lookup << 8) | ls_lookup;
		int y = registers[(size_t)RegId::Y]->get();
		int second = (first + y) % Memory::CPU_MEM_SIZE;

		int first_page = first / 256;
		int second_page = second / 256;

		if (ins->extra_cycle && first_page != second_page)
			add_extra_cycle = true;

		if(logger::CPU_TEST_MODE)
			output_string = "($" + utility::int_to_hex(fetched) + "),Y = " + utility::int_to_hex(first) + " @ " + utility::int_to_hex(second) + " = " + utility::int_to_hex(mem->ReadCPUSafe(second));

		return second;
	}
	default:
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Unknown addressing " + std::to_string((size_t)ins->mode));
		return 0;
	}

	}
}

void Cpu::PollReset(Memory *mem)
{
	if (reset) //todo: actual implementation after getting rid of CPU test
	{
		logger::PrintLine(logger::LogType::INFO, "CPU RESET detected");
		AddCycles(7);
		int initial_pc = mem->ReadCPU(0xFFFD)*256 + mem->ReadCPU(0xFFFC); //normally we jump to this
		registers[(size_t)RegId::PC]->set(initial_pc);
		reset = false;
	}
}

void Cpu::PollNMI(Memory *mem)
{
	if (mem->trigger_nmi_interrupt)
	{
		//logger::PrintLine(logger::LogType::INFO, "NMI INTERRUPT detected");
		AddCycles(7);
		mem->trigger_nmi_interrupt = false;
		
		auto p = registers[(size_t)RegId::P];
		auto sp = registers[(size_t)RegId::SP];
		auto pc = registers[(size_t)RegId::PC];
		//p->set_flag(flags::Flags::B);
		uint8_t ms = (pc->get() >> 8) & 0xFF;
		uint8_t ls = pc->get() & 0xFF;

		//push pc on stack
		mem->WriteCPU(sp->get()+STACK_OFFSET, ms);
		sp->decrement();
		mem->WriteCPU(sp->get()+STACK_OFFSET, ls);
		sp->decrement();

		//push flags on stack
		int to_write = p->get();
		utility::SetBit(&to_write, 5, 1);
		utility::SetBit(&to_write, 4, 0);
		mem->WriteCPU(sp->get()+STACK_OFFSET, to_write);
		sp->decrement();

		//jump to NMI addr
		uint8_t new_ls = mem->ReadCPU(0xFFFA);
		uint8_t new_ms = mem->ReadCPU(0xFFFB);
		int new_pc = (new_ms << 8) | new_ls;
		pc->set(new_pc);
		p->set_flag(flags::Flags::I);
		
	}
}

void Cpu::ExecuteInstruction(Memory *mem)
{
	PollReset(mem);
	PollNMI(mem);
	int old_pc = registers[(size_t)RegId::PC]->get();

	uint8_t opcode = Fetch(mem);
	auto instruction = GetInstruction(opcode);

	int value = ResolveAddressing(mem, instruction);

	if (logger::CPU_TEST_MODE)
	{
		std::stringstream ss;
		ss << utility::int_to_hex(old_pc) << "  " << std::setw(10) << std::left << GetFetchBuffer() << instruction->name << " " << std::setw(28) << std::left << output_string << RegistersToString() << PPUCounterToString() << " " << CYCToString() << std::endl;
		logger::cpu_test_buffer.push_back(ss.str());
	}

	instruction->func(this, mem, value, instruction->mode);

	AddCycles(instruction->cycles);

	if (mem->add_dma_cycles)
	{
		mem->add_dma_cycles = false;
		if (cycle_counter % 2 == 1)
			AddCycles(514);
		else
			AddCycles(513);
	}
}

uint64_t Cpu::GetCycles()
{
	return cycle_counter;
}

void Cpu::AddCycles(uint32_t num)
{
	cycle_counter += num;
	if (add_extra_cycle)
	{ 
		cycle_counter += 1;
		add_extra_cycle = false;
	}
}

void Cpu::InitRegisters()
{
	for (size_t i = 0; i < registers.size();i++)
	{
		if ((RegId)i == RegId::PC)
		{
			registers[i] = new Register(RegType::BIT16);
		}
		else if ((RegId)i == RegId::P)
		{
			registers[i] = new Register(RegType::FLAG);
		}
		else
		{
			registers[i] = new Register(RegType::BIT8);
		}
	}

	registers[(size_t)RegId::SP]->set(0xFD);
	registers[(size_t)RegId::P]->set_flag(flags::Flags::B);
	registers[(size_t)RegId::P]->set_flag(flags::Flags::I);
}

void Cpu::AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t bytes, uint8_t cycles, void (*f)(Cpu*, Memory*, int, AddressingMode mode), bool extra_cycle)
{
	if (instruction_set[opcode] != nullptr)
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Cpu::AddInstruction - trying to add an instruction with same opcode (name: " + name + " | op: " + utility::int_to_hex(opcode) + ")");
		return;
	}

	auto *instruction = new Instruction(opcode, name, mode, bytes, cycles, f, extra_cycle);
	instruction_set[opcode] = instruction;
}

Instruction* Cpu::GetInstruction(uint8_t opcode)
{
	if (instruction_set[opcode] != nullptr)
	{
		return instruction_set[opcode];
	}
	else
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Cpu::GetInstruction - unknown instruction (op: " + utility::int_to_hex(opcode) + ")");
		return nullptr;
	}
}

std::string Cpu::RegistersToString()
{
	std::string out = "";
	for (size_t i = 0; i < (size_t)RegId::PC;i++)
	{
		std::string name = Register::to_string((RegId)i);
		out += name + ":" + utility::int_to_hex(registers[i]->get(), false) + " ";
	}
	return out;
}

std::string Cpu::PPUCounterToString()
{
	uint64_t first = (cycle_counter * 3) / 341;
	uint64_t second = (cycle_counter * 3) % 341;
	std::stringstream ss;
	std::stringstream ss2;
	ss << std::setw(3) << first;
	ss2 << std::setw(3) << second;
	return "PPU:" + ss.str() + "," + ss2.str();
}

std::string Cpu::CYCToString()
{
	return "CYC:" + std::to_string(cycle_counter);
}

std::string Cpu::GetFetchBuffer()
{
	std::string fb = fetch_buffer;
	fetch_buffer = "";
	return fb;
}