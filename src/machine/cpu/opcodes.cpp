#include "opcodes.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"


namespace opcodes
{
	void LDA(Cpu* cpu, Memory *mem, int value)
	{
		if (value == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(value,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;

		cpu->registers[static_cast<size_t>(RegId::A)]->set(value);
	}

	void LDX(Cpu* cpu, Memory *mem, int value)
	{
		if (value == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(value,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;

		cpu->registers[static_cast<size_t>(RegId::X)]->set(value);
	}

	void LDY(Cpu* cpu, Memory *mem, int value)
	{
		if (value == 0)
			cpu->flags[static_cast<size_t>(flags::Flags::Z)] = true;

		if(utility::IsBitSet(value,7))
			cpu->flags[static_cast<size_t>(flags::Flags::N)] = true;

		cpu->registers[static_cast<size_t>(RegId::Y)]->set(value);
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
}