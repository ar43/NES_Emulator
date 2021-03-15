#pragma once
#include "cpu.h"

namespace opcodes
{
	void LDA(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void LDX(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void LDY(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void STA(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void STX(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void STY(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void TAX(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void TAY(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void TXA(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void TYA(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void TSX(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void TXS(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void PHA(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void PHP(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void PLA(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void PLP(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void AND(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void EOR(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void ORA(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BIT(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void ADC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void SBC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void CMP(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void CPX(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void CPY(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void INC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void INX(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void INY(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void DEC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void DEX(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void DEY(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void ASL(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void LSR(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void ROL(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void ROR(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void JMP(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void JSR(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void RTS(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void BCC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BCS(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BEQ(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BMI(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BNE(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BPL(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BVC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void BVS(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void CLC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void CLD(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void CLI(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void CLV(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void SEC(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void SED(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void SEI(Cpu* cpu, Bus *bus, int value, AddressingMode mode);

	void BRK(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void NOP(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
	void RTI(Cpu* cpu, Bus *bus, int value, AddressingMode mode);
}