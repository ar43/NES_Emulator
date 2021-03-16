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
	auto loc = *w % 2;
	addr[loc] = value;
	if (loc == 0)
	{
		const int temp_t = *t & 0xff;
		const int temp_value = (value & 0x3F) << 8;
		*t = temp_t | temp_value;
	}
	else if (loc == 1)
	{
		const int temp_t = *t & 0xff00;
		*t = temp_t | value;
		*v = *t;

		scrolladdr[0] = ((*v & 0x1f) << 3) | *x;
		scrolladdr[1] = ((*v & 0x7000) >> 12) | ((*v & 0x3e0)>>2);

	}
	*w = *w + 1;
	
}

void PpuAddr::Clear()
{
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