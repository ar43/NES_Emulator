#pragma once
#include "cpu.h"
#include "../memory/memory.h"

namespace opcodes
{
	void LDA(Cpu* cpu, Memory *mem, int value);
}