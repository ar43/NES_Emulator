#include "opcodes.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"


namespace opcodes
{
	void LDA(Cpu* cpu, Memory *mem, int value)
	{
		int new_val = mem->Read(value);
		cpu->registers[static_cast<size_t>(RegId::A)]->set(new_val);

		if (new_val == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(new_val,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;
	}

	void LDX(Cpu* cpu, Memory *mem, int value)
	{
		int new_val = mem->Read(value);
		cpu->registers[static_cast<size_t>(RegId::X)]->set(new_val);

		if (new_val == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(new_val,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;
	}

	void LDY(Cpu* cpu, Memory *mem, int value)
	{
		int new_val = mem->Read(value);
		cpu->registers[static_cast<size_t>(RegId::Y)]->set(new_val);

		if (new_val == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(new_val,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;
	}

	void STA(Cpu* cpu, Memory *mem, int value)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::A)]->get());
	}

	void STX(Cpu* cpu, Memory *mem, int value)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::X)]->get());
	}

	void STY(Cpu* cpu, Memory *mem, int value)
	{
		size_t addr = static_cast<size_t>(value);
		mem->Write(addr, cpu->registers[static_cast<size_t>(RegId::Y)]->get());
	}

	void TAX(Cpu* cpu, Memory *mem, int value)
	{
		auto a = cpu->registers[(size_t)RegId::A]->get();
		cpu->registers[(size_t)RegId::X]->set(a);

		if (a == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(a,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;
	}

	void TAY(Cpu* cpu, Memory *mem, int value)
	{
		auto a = cpu->registers[(size_t)RegId::A]->get();
		cpu->registers[(size_t)RegId::Y]->set(a);

		if (a == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(a,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;
	}

	void TXA(Cpu* cpu, Memory *mem, int value)
	{
		auto x = cpu->registers[(size_t)RegId::X]->get();
		cpu->registers[(size_t)RegId::A]->set(x);

		if (x == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(x,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;
	}

	void TYA(Cpu* cpu, Memory *mem, int value)
	{
		auto y = cpu->registers[(size_t)RegId::Y]->get();
		cpu->registers[(size_t)RegId::A]->set(y);

		if (y == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(y,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;
	}

}