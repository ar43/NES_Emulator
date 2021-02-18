#pragma once
#include "flags.h"
#include <cassert>
#include <string>

enum class RegId
{
	PC,
	SP,
	A,
	X,
	Y,
	P,
	NUM_REGISTERS
};

enum class RegType
{
	BIT8,
	BIT16,
	FLAG
};

class Register
{
private:
	int value;
	RegType type;
	
public:
	Register(RegType type)
	{
		this->value = 0;
		this->type = type;
	}

	void set(int value);
	int get();

	void increment();
	void decrement();

	void set_flag(flags::Flags flag);
	void reset_flag(flags::Flags flag);
	void toggle_flag(flags::Flags flag);
	bool get_flag(flags::Flags flag);

	static std::string to_string(RegId id)
	{
		switch (id)
		{
		case RegId::PC:
			return "PC";
		case RegId::SP:
			return "SP";
		case RegId::A:
			return "A ";
		case RegId::X:
			return "X ";
		case RegId::Y:
			return "Y ";
		case RegId::P:
			return "P ";
		default:
			return "ERROR REGISTER";
		}
	}
};