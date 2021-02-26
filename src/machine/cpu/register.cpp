#include "register.h"
#include "../../logger/logger.h"

void Register::set(int value)
{
	if(type == RegType::BIT8 || type == RegType::FLAG)
		this->value = value & 0xFF;
	else
		this->value = value & 0xFFFF;
}

int Register::get()
{
	return this->value;
}

void Register::increment()
{
	value++;

	if (type == RegType::BIT8)
		value = value & 0xFF;
	else
		value = value & 0xFFFF;
}

void Register::decrement()
{
	value--;

	if (type == RegType::BIT8 && value < 0)
	{
		value += 256;
	}
}

void Register::set_flag(flags::Flags flag)
{
	assert(type == RegType::FLAG);

	int n = (int)flag;

	this->value |= 1 << n;
}

void Register::set_flag(flags::Flags flag, bool condition)
{
	assert(type == RegType::FLAG);

	/*if (flag == flags::Flags::UNUSED)
		logger::PrintLine(logger::LogType::WARNING, "Setting UNUSED flag!");*/

	if (condition)
	{
		set_flag(flag);
	}
	else
	{
		reset_flag(flag);
	}
}

void Register::reset_flag(flags::Flags flag)
{
	assert(type == RegType::FLAG);

	int n = (int)flag;
	assert(n <= 7 && n >= 0);

	this->value &= ~(1 << n);
}

void Register::toggle_flag(flags::Flags flag)
{
	assert(type == RegType::FLAG);

	/*if (flag == flags::Flags::UNUSED)
		logger::PrintLine(logger::LogType::WARNING, "Toggling UNUSED flag!");*/

	int n = (int)flag;
	assert(n <= 7 && n >= 0);

	this->value ^= 1 << n;
}

bool Register::get_flag(flags::Flags flag)
{
	assert(type == RegType::FLAG);

	/*if (flag == flags::Flags::UNUSED)
		logger::PrintLine(logger::LogType::WARNING, "Getting UNUSED flag!");*/

	int n = (int)flag;
	assert(n <= 7 && n >= 0);

	return (this->value >> n) & 1;
}