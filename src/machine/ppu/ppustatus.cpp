#include "ppustatus.h"
#include "../../utility/utility.h"

bool PpuStatus::IsBitSet(StatusBits bit)
{
	return utility::IsBitSet(value, (size_t)bit);
}

void PpuStatus::Set(uint8_t value)
{
	this->value = value;
}

uint8_t PpuStatus::Get()
{
	return value;
}

void PpuStatus::SetBit(StatusBits bit, bool val)
{
	int cur_val = this->value;
	utility::SetBit(&cur_val, (int)bit, val);
	this->value = (uint8_t)cur_val;
}