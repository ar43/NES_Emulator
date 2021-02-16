#pragma once
#include "cpu.h"
#include "../memory/memory.h"

namespace opcodes
{
	void LDA(Cpu* cpu, Memory *mem, int value);
	void LDX(Cpu* cpu, Memory *mem, int value);
	void LDY(Cpu* cpu, Memory *mem, int value);

	void STA(Cpu* cpu, Memory *mem, int value);
	void STX(Cpu* cpu, Memory *mem, int value);
	void STY(Cpu* cpu, Memory *mem, int value);
}