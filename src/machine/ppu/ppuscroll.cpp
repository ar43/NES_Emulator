#include "ppuscroll.h"
#include "../../logger/logger.h"

void PpuScroll::Write(uint8_t value)
{
	auto loc = *w % 2;
	addr[loc] = value;
	if (loc == 0)
	{
		*x = value & 7;
		const int temp_t = *t & 0x7FE0;
		const int temp_value = (value & 0xF8)>>3;
		*t = temp_t | temp_value;
	}
	else
	{
		const int temp_t = *t & 0x7FE0;
		const int temp_value_hgfed = (value & 0xF8)<<2;
		*t = (temp_t | temp_value_hgfed) | ((value & 7)<<12);
	}
	*w = *w + 1;
}

void PpuScroll::Clear()
{
	addr[0] = 0;
	addr[1] = 0;
	addr[2] = 0;
}