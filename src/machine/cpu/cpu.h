#pragma once
#include "register.h"
#include <array>

enum class Flags
{
	C,
	Z,
	I,
	D,
	B,
	O,
	N,
	NUM_FLAGS
};

class Cpu
{
public:
	Cpu();
private:
	std::array<Register, static_cast<size_t>(RegId::NUM_REGISTERS)> registers;
	std::array<bool, static_cast<size_t>(Flags::NUM_FLAGS)> flags;
};