#include "ppumask.h"
#include "../../utility/utility.h"

bool PpuMask::IsBitSet(MaskBits bit)
{
	return utility::IsBitSet(value, (size_t)bit);
}

void PpuMask::Set(uint8_t value)
{
	this->value = value;
}