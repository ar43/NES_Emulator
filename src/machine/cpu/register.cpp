#include "register.h"
#include "../../logger/logger.h"

void Register::set(int value)
{
	if(type == RegType::BIT8 || type == RegType::FLAG)
		assert(value >= 0 && value <= 0xFF);
	else
		assert(value >= 0 && value <= 0xFFFF);

	this->value = value;

	if (type == RegType::FLAG)
	{
		reset_flag(flags::Flags::UNUSED);
	}
}

int Register::get()
{
	return this->value;
}

void Register::increment()
{
	assert(type != RegType::FLAG);

	value++;

	if(type == RegType::BIT8)
		assert(value >= 0 && value <= 0xFF);
	else
		assert(value >= 0 && value <= 0xFFFF);
}

void Register::decrement()
{
	assert(type != RegType::FLAG);

	value--;

	if(type == RegType::BIT8)
		assert(value >= 0 && value <= 0xFF);
	else
		assert(value >= 0 && value <= 0xFFFF);
}

void Register::set_flag(flags::Flags flag)
{
	assert(type == RegType::FLAG);

	if (flag == flags::Flags::UNUSED)
		logger::PrintLine(logger::LogType::WARNING, "Setting UNUSED flag!");

	int n = (int)flag;
	assert(n <= 7 && n >= 0);

	this->value |= 1 << n;
}

void Register::set_flag(flags::Flags flag, bool condition)
{
	assert(type == RegType::FLAG);

	if (flag == flags::Flags::UNUSED)
		logger::PrintLine(logger::LogType::WARNING, "Setting UNUSED flag!");

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

	if (flag == flags::Flags::UNUSED)
		logger::PrintLine(logger::LogType::WARNING, "Toggling UNUSED flag!");

	int n = (int)flag;
	assert(n <= 7 && n >= 0);

	this->value ^= 1 << n;
}

bool Register::get_flag(flags::Flags flag)
{
	assert(type == RegType::FLAG);

	if (flag == flags::Flags::UNUSED)
		logger::PrintLine(logger::LogType::WARNING, "Getting UNUSED flag!");

	int n = (int)flag;
	assert(n <= 7 && n >= 0);

	return (this->value >> n) & 1;
}