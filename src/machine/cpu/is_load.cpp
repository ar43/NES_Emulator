#include "cpu.h"
#include "opcodes.h"

//reference:
//http://www.obelisk.me.uk/6502/instructions.html

void Cpu::LoadInstructionSet()
{
	LoadLSOperations();
	LoadRTOperations();
	LoadStackOperations();
	LoadLogicalOperations();
	LoadArithmeticOperations();
	LoadIDOperations();
	LoadShiftsOperations();
	LoadJCOperations();
	LoadBranchesOperations();
	LoadStatusFlagOperations();
	LoadSystemFunctionOperations();
}

void Cpu::LoadLSOperations()
{
	AddInstruction("LDA", 0xA9, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::LDA);
	AddInstruction("LDA", 0xA5, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::LDA);
	AddInstruction("LDA", 0xB5, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::LDA);
	AddInstruction("LDA", 0xAD, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::LDA);
	AddInstruction("LDA", 0xBD, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::LDA, true);
	AddInstruction("LDA", 0xB9, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::LDA, true);
	AddInstruction("LDA", 0xA1, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::LDA);
	AddInstruction("LDA", 0xB1, AddressingMode::INDIRECT_INDEXED, 2, 5, &opcodes::LDA, true);

	AddInstruction("LDX", 0xA2, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::LDX);
	AddInstruction("LDX", 0xA6, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::LDX);
	AddInstruction("LDX", 0xB6, AddressingMode::ZERO_PAGE_Y,	  2, 4, &opcodes::LDX);
	AddInstruction("LDX", 0xAE, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::LDX);
	AddInstruction("LDX", 0xBE, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::LDX, true);

	AddInstruction("LDY", 0xA0, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::LDY);
	AddInstruction("LDY", 0xA4, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::LDY);
	AddInstruction("LDY", 0xB4, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::LDY);
	AddInstruction("LDY", 0xAC, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::LDY);
	AddInstruction("LDY", 0xBC, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::LDY, true);

	AddInstruction("STA", 0x85, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::STA);
	AddInstruction("STA", 0x95, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::STA);
	AddInstruction("STA", 0x8D, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::STA);
	AddInstruction("STA", 0x9D, AddressingMode::ABSOLUTE_X,		  3, 5, &opcodes::STA);
	AddInstruction("STA", 0x99, AddressingMode::ABSOLUTE_Y,		  3, 5, &opcodes::STA);
	AddInstruction("STA", 0x81, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::STA);
	AddInstruction("STA", 0x91, AddressingMode::INDIRECT_INDEXED, 2, 6, &opcodes::STA);

	AddInstruction("STX", 0x86, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::STX);
	AddInstruction("STX", 0x96, AddressingMode::ZERO_PAGE_Y,	  2, 4, &opcodes::STX);
	AddInstruction("STX", 0x8E, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::STX);

	AddInstruction("STY", 0x84, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::STY);
	AddInstruction("STY", 0x94, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::STY);
	AddInstruction("STY", 0x8C, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::STY);

}

void Cpu::LoadRTOperations()
{
	AddInstruction("TAX", 0xAA, AddressingMode::IMPLICIT,		  1, 2, &opcodes::TAX);
	AddInstruction("TAY", 0xA8, AddressingMode::IMPLICIT,		  1, 2, &opcodes::TAY);
	AddInstruction("TXA", 0x8A, AddressingMode::IMPLICIT,		  1, 2, &opcodes::TXA);
	AddInstruction("TYA", 0x98, AddressingMode::IMPLICIT,		  1, 2, &opcodes::TYA);
	
}

void Cpu::LoadStackOperations()
{
	AddInstruction("TSX", 0xBA, AddressingMode::IMPLICIT,		  1, 2, &opcodes::TSX);
	AddInstruction("TXS", 0x9A, AddressingMode::IMPLICIT,		  1, 2, &opcodes::TXS);
	AddInstruction("PHA", 0x48, AddressingMode::IMPLICIT,		  1, 3, &opcodes::PHA);
	AddInstruction("PHP", 0x08, AddressingMode::IMPLICIT,		  1, 3, &opcodes::PHP);
	AddInstruction("PLA", 0x68, AddressingMode::IMPLICIT,		  1, 4, &opcodes::PLA);
	AddInstruction("PLP", 0x28, AddressingMode::IMPLICIT,		  1, 4, &opcodes::PLP);
}

void Cpu::LoadLogicalOperations()
{
	AddInstruction("AND", 0x29, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::AND);
	AddInstruction("AND", 0x25, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::AND);
	AddInstruction("AND", 0x35, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::AND);
	AddInstruction("AND", 0x2D, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::AND);
	AddInstruction("AND", 0x3D, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::AND, true);
	AddInstruction("AND", 0x39, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::AND, true);
	AddInstruction("AND", 0x21, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::AND);
	AddInstruction("AND", 0x31, AddressingMode::INDIRECT_INDEXED, 2, 5, &opcodes::AND, true);
	
	AddInstruction("EOR", 0x49, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::EOR);
	AddInstruction("EOR", 0x45, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::EOR);
	AddInstruction("EOR", 0x55, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::EOR);
	AddInstruction("EOR", 0x4D, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::EOR);
	AddInstruction("EOR", 0x5D, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::EOR, true);
	AddInstruction("EOR", 0x59, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::EOR, true);
	AddInstruction("EOR", 0x41, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::EOR);
	AddInstruction("EOR", 0x51, AddressingMode::INDIRECT_INDEXED, 2, 5, &opcodes::EOR, true);

	AddInstruction("ORA", 0x09, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::ORA);
	AddInstruction("ORA", 0x05, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::ORA);
	AddInstruction("ORA", 0x15, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::ORA);
	AddInstruction("ORA", 0x0D, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::ORA);
	AddInstruction("ORA", 0x1D, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::ORA, true);
	AddInstruction("ORA", 0x19, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::ORA, true);
	AddInstruction("ORA", 0x01, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::ORA);
	AddInstruction("ORA", 0x11, AddressingMode::INDIRECT_INDEXED, 2, 5, &opcodes::ORA, true);

	AddInstruction("BIT", 0x24, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::BIT);
	AddInstruction("BIT", 0x2C, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::BIT);
}

void Cpu::LoadArithmeticOperations()
{
	//for ADC - carry bit is set if the result goes over 0xFF, so you have to wrap it around
	//overflow if you add two positive and get negative or two negative and get positive
	AddInstruction("ADC", 0x69, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::ADC);
	AddInstruction("ADC", 0x65, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::ADC);
	AddInstruction("ADC", 0x75, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::ADC);
	AddInstruction("ADC", 0x6D, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::ADC);
	AddInstruction("ADC", 0x7D, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::ADC, true);
	AddInstruction("ADC", 0x79, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::ADC, true);
	AddInstruction("ADC", 0x61, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::ADC);
	AddInstruction("ADC", 0x71, AddressingMode::INDIRECT_INDEXED, 2, 5, &opcodes::ADC, true);

	AddInstruction("SBC", 0xE9, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::SBC);
	AddInstruction("SBC", 0xE5, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::SBC);
	AddInstruction("SBC", 0xF5, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::SBC);
	AddInstruction("SBC", 0xED, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::SBC);
	AddInstruction("SBC", 0xFD, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::SBC, true);
	AddInstruction("SBC", 0xF9, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::SBC, true);
	AddInstruction("SBC", 0xE1, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::SBC);
	AddInstruction("SBC", 0xF1, AddressingMode::INDIRECT_INDEXED, 2, 5, &opcodes::SBC, true);

	AddInstruction("CMP", 0xC9, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::CMP);
	AddInstruction("CMP", 0xC5, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::CMP);
	AddInstruction("CMP", 0xD5, AddressingMode::ZERO_PAGE_X,	  2, 4, &opcodes::CMP);
	AddInstruction("CMP", 0xCD, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::CMP);
	AddInstruction("CMP", 0xDD, AddressingMode::ABSOLUTE_X,		  3, 4, &opcodes::CMP, true);
	AddInstruction("CMP", 0xD9, AddressingMode::ABSOLUTE_Y,		  3, 4, &opcodes::CMP, true);
	AddInstruction("CMP", 0xC1, AddressingMode::INDEXED_INDIRECT, 2, 6, &opcodes::CMP);
	AddInstruction("CMP", 0xD1, AddressingMode::INDIRECT_INDEXED, 2, 5, &opcodes::CMP, true);

	AddInstruction("CPX", 0xE0, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::CPX);
	AddInstruction("CPX", 0xE4, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::CPX);
	AddInstruction("CPX", 0xEC, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::CPX);

	AddInstruction("CPY", 0xC0, AddressingMode::IMMEDIATE,		  2, 2, &opcodes::CPY);
	AddInstruction("CPY", 0xC4, AddressingMode::ZERO_PAGE,		  2, 3, &opcodes::CPY);
	AddInstruction("CPY", 0xCC, AddressingMode::ABSOLUTE,		  3, 4, &opcodes::CPY);

}

void Cpu::LoadIDOperations()
{
	AddInstruction("INC", 0xE6, AddressingMode::ZERO_PAGE,		  2, 5, &opcodes::INC);
	AddInstruction("INC", 0xF6, AddressingMode::ZERO_PAGE_X,	  2, 6, &opcodes::INC);
	AddInstruction("INC", 0xEE, AddressingMode::ABSOLUTE,		  3, 6, &opcodes::INC);
	AddInstruction("INC", 0xFE, AddressingMode::ABSOLUTE_X,		  3, 7, &opcodes::INC);

	AddInstruction("INX", 0xE8, AddressingMode::IMPLICIT,		  1, 2, &opcodes::INX);

	AddInstruction("INY", 0xC8, AddressingMode::IMPLICIT,		  1, 2, &opcodes::INY);

	AddInstruction("DEC", 0xC6, AddressingMode::ZERO_PAGE,		  2, 5, &opcodes::DEC);
	AddInstruction("DEC", 0xD6, AddressingMode::ZERO_PAGE_X,	  2, 6, &opcodes::DEC);
	AddInstruction("DEC", 0xCE, AddressingMode::ABSOLUTE,		  3, 6, &opcodes::DEC);
	AddInstruction("DEC", 0xDE, AddressingMode::ABSOLUTE_X,		  3, 7, &opcodes::DEC);

	AddInstruction("DEX", 0xCA, AddressingMode::IMPLICIT,		  1, 2, &opcodes::DEX);

	AddInstruction("DEY", 0x88, AddressingMode::IMPLICIT,		  1, 2, &opcodes::DEY);
}

void Cpu::LoadShiftsOperations()
{
	AddInstruction("ASL", 0x0A, AddressingMode::ACCUMULATOR,	  1, 2, &opcodes::ASL);
	AddInstruction("ASL", 0x06, AddressingMode::ZERO_PAGE,		  2, 5, &opcodes::ASL);
	AddInstruction("ASL", 0x16, AddressingMode::ZERO_PAGE_X,	  2, 6, &opcodes::ASL);
	AddInstruction("ASL", 0x0E, AddressingMode::ABSOLUTE,		  3, 6, &opcodes::ASL);
	AddInstruction("ASL", 0x1E, AddressingMode::ABSOLUTE_X,		  3, 7, &opcodes::ASL);

	AddInstruction("LSR", 0x4A, AddressingMode::ACCUMULATOR,	  1, 2, &opcodes::LSR);
	AddInstruction("LSR", 0x46, AddressingMode::ZERO_PAGE,		  2, 5, &opcodes::LSR);
	AddInstruction("LSR", 0x56, AddressingMode::ZERO_PAGE_X,	  2, 6, &opcodes::LSR);
	AddInstruction("LSR", 0x4E, AddressingMode::ABSOLUTE,		  3, 6, &opcodes::LSR);
	AddInstruction("LSR", 0x5E, AddressingMode::ABSOLUTE_X,		  3, 7, &opcodes::LSR);

	AddInstruction("ROL", 0x2A, AddressingMode::ACCUMULATOR,	  1, 2, &opcodes::ROL);
	AddInstruction("ROL", 0x26, AddressingMode::ZERO_PAGE,		  2, 5, &opcodes::ROL);
	AddInstruction("ROL", 0x36, AddressingMode::ZERO_PAGE_X,	  2, 6, &opcodes::ROL);
	AddInstruction("ROL", 0x2E, AddressingMode::ABSOLUTE,		  3, 6, &opcodes::ROL);
	AddInstruction("ROL", 0x3E, AddressingMode::ABSOLUTE_X,		  3, 7, &opcodes::ROL);

	AddInstruction("ROR", 0x6A, AddressingMode::ACCUMULATOR,	  1, 2, &opcodes::ROR);
	AddInstruction("ROR", 0x66, AddressingMode::ZERO_PAGE,		  2, 5, &opcodes::ROR);
	AddInstruction("ROR", 0x76, AddressingMode::ZERO_PAGE_X,	  2, 6, &opcodes::ROR);
	AddInstruction("ROR", 0x6E, AddressingMode::ABSOLUTE,		  3, 6, &opcodes::ROR);
	AddInstruction("ROR", 0x7E, AddressingMode::ABSOLUTE_X,		  3, 7, &opcodes::ROR);
}

void Cpu::LoadJCOperations()
{
	AddInstruction("JMP", 0x4C, AddressingMode::ABSOLUTE,	  3, 3, &opcodes::JMP);
	AddInstruction("JMP", 0x6C, AddressingMode::INDIRECT,	  3, 5, &opcodes::JMP);

	AddInstruction("JSR", 0x20, AddressingMode::ABSOLUTE,	  3, 6, &opcodes::JSR);

	AddInstruction("RTS", 0x60, AddressingMode::IMPLICIT,	  1, 6, &opcodes::RTS);
}

void Cpu::LoadBranchesOperations()
{
	AddInstruction("BCC", 0x90, AddressingMode::RELATIVE,	  2, 2, &opcodes::BCC);
	AddInstruction("BCS", 0xB0, AddressingMode::RELATIVE,	  2, 2, &opcodes::BCS);
	AddInstruction("BEQ", 0xF0, AddressingMode::RELATIVE,	  2, 2, &opcodes::BEQ);
	AddInstruction("BMI", 0x30, AddressingMode::RELATIVE,	  2, 2, &opcodes::BMI);
	AddInstruction("BNE", 0xD0, AddressingMode::RELATIVE,	  2, 2, &opcodes::BNE);
	AddInstruction("BPL", 0x10, AddressingMode::RELATIVE,	  2, 2, &opcodes::BPL);
	AddInstruction("BVC", 0x50, AddressingMode::RELATIVE,	  2, 2, &opcodes::BVC);
	AddInstruction("BVS", 0x70, AddressingMode::RELATIVE,	  2, 2, &opcodes::BVS);
}

void Cpu::LoadStatusFlagOperations()
{
	AddInstruction("CLC", 0x18, AddressingMode::IMPLICIT,	  1, 2, &opcodes::CLC);
	AddInstruction("CLD", 0xD8, AddressingMode::IMPLICIT,	  1, 2, &opcodes::CLD);
	AddInstruction("CLI", 0x58, AddressingMode::IMPLICIT,	  1, 2, &opcodes::CLI);
	AddInstruction("CLV", 0xB8, AddressingMode::IMPLICIT,	  1, 2, &opcodes::CLV);
	AddInstruction("SEC", 0x38, AddressingMode::IMPLICIT,	  1, 2, &opcodes::SEC);
	AddInstruction("SED", 0xF8, AddressingMode::IMPLICIT,	  1, 2, &opcodes::SED);
	AddInstruction("SEI", 0x78, AddressingMode::IMPLICIT,	  1, 2, &opcodes::SEI);
}

void Cpu::LoadSystemFunctionOperations()
{
	AddInstruction("BRK", 0x00, AddressingMode::IMPLICIT,	  1, 7, &opcodes::BRK);
	AddInstruction("NOP", 0xEA, AddressingMode::IMPLICIT,	  1, 2, &opcodes::NOP);
	AddInstruction("RTI", 0x40, AddressingMode::IMPLICIT,	  1, 6, &opcodes::RTI);
}