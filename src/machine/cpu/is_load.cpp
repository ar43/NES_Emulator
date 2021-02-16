#include "cpu.h"
#include "opcodes.h"

void Cpu::LoadInstructionSet()
{
	LoadLSOperations();
	LoadRTOperations();
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

}