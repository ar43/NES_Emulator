#include "ppuctrl.h"
#include "../../utility/utility.h"

//int GetNametable();
//bool IsBitSet(ControllerBits bit);
//void Set();

int PpuCtrl::GetNametable()
{
	int lookup = value & 3;
	if (lookup == 0)
		return 0x2000;
	else if (lookup == 1)
		return 0x2400;
	else if (lookup == 2)
		return 0x2800;
	else
		return 0x2C00;
}

bool PpuCtrl::IsBitSet(ControllerBits bit)
{
	return utility::IsBitSet(value, (size_t)bit);
}

void PpuCtrl::Set(uint8_t value)
{
	this->value = value;
}