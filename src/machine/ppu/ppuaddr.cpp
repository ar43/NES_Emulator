#include "ppuaddr.h"
#include "../../utility/utility.h"

//int GetAddr();
//int Write();
//int Increment();

int PpuAddr::GetAddr()
{
	int ret = addr[0] << 8 | addr[1];
	return ret;
}

void PpuAddr::Write(uint8_t value, int *t, int *v)
{
	auto loc = counter % 2;
	addr[loc] = value;
	counter++;
	if (loc == 0)
	{
		utility::SetBit(t, 10, utility::IsBitSet(value, 2));
		utility::SetBit(t, 10, utility::IsBitSet(value, 3));
	}
	else if (loc == 1)
	{
		*v = *t;
	}
}

void PpuAddr::ClearCounter()
{
	counter = 0;
}

void PpuAddr::Clear()
{
	counter = 0;
	addr[0] = 0;
	addr[1] = 0;
}

void PpuAddr::Add(uint8_t val)
{
	int current_addr = GetAddr();
	current_addr += val;
	current_addr &= 0x3FFF;
	addr[0] = (current_addr >> 8) & 0xFF;
	addr[1] = current_addr & 0xFF;
}