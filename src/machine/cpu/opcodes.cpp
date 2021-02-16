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
}