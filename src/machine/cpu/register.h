#pragma once
#include <cassert>
#include <string>

enum class RegId
{
	PC,
	SR,
	A,
	X,
	Y,
	NUM_REGISTERS
};

class Register
{
private:
	int value;
	
public:
	Register()
	{
		value = 0;
	}

	void set(int value)
	{
		assert(value >= 0 && value <= 0xFF);
		this->value = value;
	}

	int get()
	{
		return this->value;
	}

	static std::string to_string(RegId id)
	{
		switch (id)
		{
		case RegId::PC:
			return "PC";
		case RegId::SR:
			return "SR";
		case RegId::A:
			return "A";
		case RegId::X:
			return "X";
		case RegId::Y:
			return "Y";
		default:
			return "ERROR REGISTER";
		}
	}
};