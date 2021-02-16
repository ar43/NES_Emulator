#pragma once
#include <cassert>
#include <string>

enum class RegId
{
	PC,
	SP,
	A,
	X,
	Y,
	NUM_REGISTERS
};

enum class RegType
{
	BIT8,
	BIT16
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

	void set(int value)
	{
		if(type == RegType::BIT8)
			assert(value >= 0 && value <= 0xFF);
		else
			assert(value >= 0 && value <= 0xFFFF);
		this->value = value;
	}

	int get()
	{
		return this->value;
	}

	void increment()
	{
		value++;

		if(type == RegType::BIT8)
			assert(value >= 0 && value <= 0xFF);
		else
			assert(value >= 0 && value <= 0xFFFF);
	}

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
		default:
			return "ERROR REGISTER";
		}
	}
};