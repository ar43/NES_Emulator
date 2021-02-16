#pragma once
enum class AddressingMode
{
	IMPLICIT,
	ACCUMULATOR,
	IMMEDIATE,
	ZERO_PAGE,
	ZERO_PAGE_X,
	ZERO_PAGE_Y, 
	RELATIVE, //Relative (offset from following instruction)
	ABSOLUTE, //Absolute
	ABSOLUTE_X, //Absolute,X
	ABSOLUTE_Y, //Absolute,Y
	INDIRECT, ///(Indirect)
	INDEXED_INDIRECT, //(Indirect,X)
	INDIRECT_INDEXED  //(Indirect),Y
};