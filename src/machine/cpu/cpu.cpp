#include "cpu.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include "../bus/bus.h"
#include "../misc/machine_status.h"
#include "../user_interface/debugger.h"
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

void Cpu::WriteRam(size_t loc, uint8_t byte)
{
	if (loc >= 0x0800 && loc <= 0x1FFF)
	{
		loc &= 0x07FF;
	}

	RAM[loc] = byte;
}

uint8_t Cpu::ReadRam(size_t loc)
{
	if (loc >= 0x0800 && loc <= 0x1FFF)
	{
		loc &= 0x07FF;
	}

	return RAM[loc];
}

uint8_t Cpu::Fetch(Bus* bus, bool generate_string)
{
	uint8_t ret = bus->ReadCPUSafe(registers[(size_t)RegId::PC]->get());
	registers[(size_t)RegId::PC]->increment();

	if (logger::CPU_TEST_MODE || generate_string)
	{
		fetch_buffer += utility::int_to_hex(ret) + " ";
	}

	return ret;
}

int Cpu::ResolveAddressing(Bus* bus, Instruction* ins, bool generate_string)
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
		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "A";
		return 0;
	}
	case AddressingMode::IMMEDIATE:
	{
		auto fetched = Fetch(bus, generate_string);
		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "#$" + utility::int_to_hex(fetched);
		return fetched;
	}
	case AddressingMode::ZERO_PAGE:
	{
		auto fetched = Fetch(bus, generate_string);
		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "$" + utility::int_to_hex(fetched) + " = " + utility::int_to_hex(bus->ReadCPUSafe(fetched));
		return fetched;
	}
	case AddressingMode::ZERO_PAGE_X:
	{
		uint8_t fetched = Fetch(bus, generate_string);
		int x = registers[(size_t)RegId::X]->get();
		int addr = fetched + x;
		addr = addr % 0x100;
		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "$" + utility::int_to_hex(fetched) + ",X @ " + utility::int_to_hex(addr) + " = " + utility::int_to_hex(bus->ReadCPUSafe(addr));
		return addr;
	}
	case AddressingMode::ZERO_PAGE_Y:
	{
		uint8_t fetched = Fetch(bus, generate_string);
		int y = registers[(size_t)RegId::Y]->get();
		int addr = fetched + y;
		addr = addr % 0x100;
		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "$" + utility::int_to_hex(fetched) + ",Y @ " + utility::int_to_hex(addr) + " = " + utility::int_to_hex(bus->ReadCPUSafe(addr));
		return addr;
	}
	case AddressingMode::RELATIVE:
	{
		uint8_t fetched = Fetch(bus, generate_string);
		int ret = (int8_t)fetched;
		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "$" + utility::int_to_hex(ret + registers[(size_t)RegId::PC]->get());
		return ret;
	}
	case AddressingMode::ABSOLUTE:
	{
		uint8_t ls = Fetch(bus, generate_string);
		uint8_t ms = Fetch(bus, generate_string);
		int ret = (ms << 8) | ls;

		if((logger::CPU_TEST_MODE || generate_string) && ins->opcode != 0x20 && ins->opcode != 0x4C)
			output_string = "$" + utility::int_to_hex(ret) + " = " + utility::int_to_hex(bus->ReadCPUSafe(ret));
		else if(logger::CPU_TEST_MODE || generate_string)
			output_string = "$" + utility::int_to_hex(ret);

		return ret;
	}
	case AddressingMode::ABSOLUTE_X: //extra cycle support
	{
		uint8_t ls = Fetch(bus, generate_string);
		uint8_t ms = Fetch(bus, generate_string);
		int first = (ms << 8) | ls;

		int x = registers[(size_t)RegId::X]->get();
		int second = (first + x) % CPU_MEM_SIZE;

		int first_page = first / 256;
		int second_page = second / 256;

		if (ins->extra_cycle && first_page != second_page)
			add_extra_cycle = true;

		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "$" + utility::int_to_hex(first) + ",X @ " + utility::int_to_hex(second) + " = " + utility::int_to_hex(bus->ReadCPUSafe(second));
		return second;
	}
	case AddressingMode::ABSOLUTE_Y: //extra cycle support
	{
		uint8_t ls = Fetch(bus, generate_string);
		uint8_t ms = Fetch(bus, generate_string);
		int first = (ms << 8) | ls;

		int y = registers[(size_t)RegId::Y]->get();
		int second = (first + y) % CPU_MEM_SIZE;

		int first_page = first / 256;
		int second_page = second / 256;

		if (ins->extra_cycle && first_page != second_page)
			add_extra_cycle = true;

		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "$" + utility::int_to_hex(first) + ",Y @ " + utility::int_to_hex(second) + " = " + utility::int_to_hex(bus->ReadCPUSafe(second));
		return second;
	}
	case AddressingMode::INDIRECT:
	{
		uint8_t ls = Fetch(bus, generate_string);
		uint8_t ms = Fetch(bus, generate_string);
		size_t addr = (ms << 8) | ls;

		assert(addr < 0xFFFF);
		uint8_t ls_lookup = bus->ReadCPUSafe(addr);
		int old_addr = addr;
		addr = addr + 1;
		if (addr % 0x100 == 0)
			addr -= 0x100;
		uint8_t ms_lookup = bus->ReadCPUSafe(addr);

		int ret = (ms_lookup << 8) | ls_lookup;

		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "($" + utility::int_to_hex(old_addr) + ") = " + utility::int_to_hex(ret);

		return ret;
	}
	case AddressingMode::INDEXED_INDIRECT:
	{
		uint8_t fetched = Fetch(bus, generate_string);
		int x = registers[(size_t)RegId::X]->get();
		int addr = fetched + x;
		if (addr > 0xFF)
			addr = addr % 0x100;

		uint8_t ls_lookup = bus->ReadCPUSafe(addr);
		int old_addr = addr;
		addr = (addr + 1) % 0x100;
		uint8_t ms_lookup = bus->ReadCPUSafe(addr);

		int ret = (ms_lookup << 8) | ls_lookup;

		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "($" + utility::int_to_hex(fetched) + ",X) @ " + utility::int_to_hex(old_addr) + " = " + utility::int_to_hex(ret) + " = " + utility::int_to_hex(bus->ReadCPUSafe(ret));

		return ret;
	}
	case AddressingMode::INDIRECT_INDEXED: //extra cycle support
	{
		uint8_t fetched = Fetch(bus, generate_string);
		int addr = fetched;
		uint8_t ls_lookup = bus->ReadCPUSafe(addr);
		addr= (addr + 1) % 0x100;
		uint8_t ms_lookup = bus->ReadCPUSafe(addr);

		int first = (ms_lookup << 8) | ls_lookup;
		int y = registers[(size_t)RegId::Y]->get();
		int second = (first + y) % CPU_MEM_SIZE;

		int first_page = first / 256;
		int second_page = second / 256;

		if (ins->extra_cycle && first_page != second_page)
			add_extra_cycle = true;

		if(logger::CPU_TEST_MODE || generate_string)
			output_string = "($" + utility::int_to_hex(fetched) + "),Y = " + utility::int_to_hex(first) + " @ " + utility::int_to_hex(second) + " = " + utility::int_to_hex(bus->ReadCPUSafe(second));

		return second;
	}
	default:
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Unknown addressing " + std::to_string((size_t)ins->mode));
		return 0;
	}

	}
}

void Cpu::HandleReset(Bus *bus, ResetType reset)
{
	
	logger::PrintLine(logger::LogType::INFO, "CPU RESET detected");
	AddCycles(7);
	int initial_pc = bus->ReadCPU(0xFFFD)*256 + bus->ReadCPU(0xFFFC); //normally we jump to this
	debug_data->all_bytes = 0xffff - initial_pc + 1;
	registers[(size_t)RegId::PC]->set(initial_pc);

	if (reset == ResetType::HARD)
		registers[(size_t)RegId::SP]->set(0xFD);
	else
		registers[(size_t)RegId::SP]->set(registers[(size_t)RegId::SP]->get() - 3);

	registers[(size_t)RegId::P]->set_flag(flags::Flags::I);
}

void Cpu::HandleNMI(Bus *bus)
{
	//logger::PrintLine(logger::LogType::INFO, "NMI INTERRUPT detected");
	AddCycles(7);
		
	auto p = registers[(size_t)RegId::P];
	auto sp = registers[(size_t)RegId::SP];
	auto pc = registers[(size_t)RegId::PC];
	//p->set_flag(flags::Flags::B);
	uint8_t ms = (pc->get() >> 8) & 0xFF;
	uint8_t ls = pc->get() & 0xFF;

	//push pc on stack
	bus->WriteCPU(sp->get()+STACK_OFFSET, ms);
	sp->decrement();
	bus->WriteCPU(sp->get()+STACK_OFFSET, ls);
	sp->decrement();

	//push flags on stack
	int to_write = p->get();
	utility::SetBit(&to_write, 5, 1);
	utility::SetBit(&to_write, 4, 0);
	bus->WriteCPU(sp->get()+STACK_OFFSET, to_write);
	sp->decrement();

	//jump to NMI addr
	uint8_t new_ls = bus->ReadCPU(0xFFFA);
	uint8_t new_ms = bus->ReadCPU(0xFFFB);
	int new_pc = (new_ms << 8) | new_ls;
	pc->set(new_pc);
	p->set_flag(flags::Flags::I);
}

void Cpu::HandleIRQ(Bus *bus)
{
	//logger::PrintLine(logger::LogType::INFO, "NMI INTERRUPT detected");
	AddCycles(7);

	auto p = registers[(size_t)RegId::P];
	auto sp = registers[(size_t)RegId::SP];
	auto pc = registers[(size_t)RegId::PC];
	//p->set_flag(flags::Flags::B);
	uint8_t ms = (pc->get() >> 8) & 0xFF;
	uint8_t ls = pc->get() & 0xFF;

	//push pc on stack
	bus->WriteCPU(sp->get()+STACK_OFFSET, ms);
	sp->decrement();
	bus->WriteCPU(sp->get()+STACK_OFFSET, ls);
	sp->decrement();

	//push flags on stack
	int to_write = p->get();
	utility::SetBit(&to_write, 5, 1);
	utility::SetBit(&to_write, 4, 0);
	bus->WriteCPU(sp->get()+STACK_OFFSET, to_write);
	sp->decrement();

	//jump to IRQ addr
	uint8_t new_ls = bus->ReadCPU(0xFFFE);
	uint8_t new_ms = bus->ReadCPU(0xFFFF);
	int new_pc = (new_ms << 8) | new_ls;
	pc->set(new_pc);
	p->set_flag(flags::Flags::I);
}

void Cpu::ExecuteInstruction(Bus *bus)
{
	bool generate_string = false;

	int old_pc = registers[(size_t)RegId::PC]->get();

	if (debug_mode)
	{
		if (debug_data->code[old_pc].empty())
			generate_string = true;
	}

	uint8_t opcode = Fetch(bus, generate_string);
	auto instruction = GetInstruction(opcode);

	int value = ResolveAddressing(bus, instruction, generate_string);

	//if (logger::CPU_TEST_MODE)
	//{
	//	std::stringstream ss;
	//	//ss << utility::int_to_hex(old_pc) << "  " << std::setw(10) << std::left << GetFetchBuffer() << instruction->name << " " << std::setw(28) << std::left << output_string << RegistersToString() << PPUCounterToString() << " " << CYCToString() << std::endl;
	//	ss << utility::int_to_hex(old_pc) << "  " << std::setw(10) << std::left << GetFetchBuffer() << instruction->name << " " << std::setw(28) << std::left << output_string << std::endl;
	//	logger::cpu_test_buffer.push_back(ss.str());
	//	output_string = "";
	//}

	if (generate_string)
	{
		std::stringstream ss;
		ss << utility::int_to_hex(old_pc) << "  " << std::setw(10) << std::left << GetFetchBuffer() << instruction->name << " " << std::setw(28) << std::left << output_string << std::endl;
		debug_data->code[old_pc] = ss.str();
		debug_data->known_bytes += registers[(size_t)RegId::PC]->get() - old_pc;
		//logger::PrintLine(logger::LogType::DEBUG, std::to_string(double(debug_data->known_bytes)/double(debug_data->all_bytes)*100) + "% = " + std::to_string(debug_data->known_bytes) + "/" + std::to_string(debug_data->all_bytes));
		output_string = "";
	}

	instruction->func(this, bus, value, instruction->mode);

	AddCycles(instruction->cycles);

	if (bus->add_dma_cycles)
	{
		bus->add_dma_cycles = false;
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

void Cpu::Init(DebugData *debug_data, bool **debug_mode)
{
	this->debug_data = debug_data;
	*debug_mode = &this->debug_mode;
}

void Cpu::InitRegisters()
{
	for (size_t i = 0; i < static_cast<size_t>(RegId::NUM_REGISTERS);i++)
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
	registers[(size_t)RegId::P]->set_flag(flags::Flags::UNUSED);
	registers[(size_t)RegId::P]->set_flag(flags::Flags::I);
}

void Cpu::AddInstruction(std::string name, uint8_t opcode, AddressingMode mode, uint8_t bytes, uint8_t cycles, void (*f)(Cpu*, Bus*, int, AddressingMode mode), bool extra_cycle)
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
	uint64_t first = ((cycle_counter * 3) / 341) & 261;
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