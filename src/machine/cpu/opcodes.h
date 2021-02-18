#pragma once
#include "cpu.h"
#include "../memory/memory.h"

namespace opcodes
{
	void LDA(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void LDX(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void LDY(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void STA(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void STX(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void STY(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void TAX(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void TAY(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void TXA(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void TYA(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void TSX(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void TXS(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void PHA(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void PHP(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void PLA(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void PLP(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void AND(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void EOR(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void ORA(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BIT(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void ADC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void SBC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void CMP(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void CPX(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void CPY(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void INC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void INX(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void INY(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void DEC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void DEX(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void DEY(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void ASL(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void LSR(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void ROL(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void ROR(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void JMP(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void JSR(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void RTS(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void BCC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BCS(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BEQ(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BMI(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BNE(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BPL(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BVC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void BVS(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void CLC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void CLD(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void CLI(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void CLV(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void SEC(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void SED(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void SEI(Cpu* cpu, Memory *mem, int value, AddressingMode mode);

	void BRK(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void NOP(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
	void RTI(Cpu* cpu, Memory *mem, int value, AddressingMode mode);
}