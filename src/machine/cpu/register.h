#pragma once
#include <cassert>

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
};