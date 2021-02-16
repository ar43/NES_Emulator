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

	void TAX(Cpu* cpu, Memory *mem, int value);
	void TAY(Cpu* cpu, Memory *mem, int value);
	void TXA(Cpu* cpu, Memory *mem, int value);
	void TYA(Cpu* cpu, Memory *mem, int value);

	void TSX(Cpu* cpu, Memory *mem, int value);
	void TXS(Cpu* cpu, Memory *mem, int value);
	void PHA(Cpu* cpu, Memory *mem, int value);
	void PHP(Cpu* cpu, Memory *mem, int value);
	void PLA(Cpu* cpu, Memory *mem, int value);
	void PLP(Cpu* cpu, Memory *mem, int value);

	void AND(Cpu* cpu, Memory *mem, int value);
	void EOR(Cpu* cpu, Memory *mem, int value);
	void ORA(Cpu* cpu, Memory *mem, int value);
	void BIT(Cpu* cpu, Memory *mem, int value);

	void ADC(Cpu* cpu, Memory *mem, int value);
	void SBC(Cpu* cpu, Memory *mem, int value);
	void CMP(Cpu* cpu, Memory *mem, int value);
	void CPX(Cpu* cpu, Memory *mem, int value);
	void CPY(Cpu* cpu, Memory *mem, int value);

	void INC(Cpu* cpu, Memory *mem, int value);
	void INX(Cpu* cpu, Memory *mem, int value);
	void INY(Cpu* cpu, Memory *mem, int value);
	void DEC(Cpu* cpu, Memory *mem, int value);
	void DEX(Cpu* cpu, Memory *mem, int value);
	void DEY(Cpu* cpu, Memory *mem, int value);

	void ASL(Cpu* cpu, Memory *mem, int value);
	void LSR(Cpu* cpu, Memory *mem, int value);
	void ROL(Cpu* cpu, Memory *mem, int value);
	void ROR(Cpu* cpu, Memory *mem, int value);

	void JMP(Cpu* cpu, Memory *mem, int value);
	void JSR(Cpu* cpu, Memory *mem, int value);
	void RTS(Cpu* cpu, Memory *mem, int value);

	void BCC(Cpu* cpu, Memory *mem, int value);
	void BCS(Cpu* cpu, Memory *mem, int value);
	void BEQ(Cpu* cpu, Memory *mem, int value);
	void BMI(Cpu* cpu, Memory *mem, int value);
	void BNE(Cpu* cpu, Memory *mem, int value);
	void BPL(Cpu* cpu, Memory *mem, int value);
	void BVC(Cpu* cpu, Memory *mem, int value);
	void BVS(Cpu* cpu, Memory *mem, int value);

	void CLC(Cpu* cpu, Memory *mem, int value);
	void CLD(Cpu* cpu, Memory *mem, int value);
	void CLI(Cpu* cpu, Memory *mem, int value);
	void CLV(Cpu* cpu, Memory *mem, int value);
	void SEC(Cpu* cpu, Memory *mem, int value);
	void SED(Cpu* cpu, Memory *mem, int value);
	void SEI(Cpu* cpu, Memory *mem, int value);

	void BRK(Cpu* cpu, Memory *mem, int value);
	void NOP(Cpu* cpu, Memory *mem, int value);
	void RTI(Cpu* cpu, Memory *mem, int value);
}