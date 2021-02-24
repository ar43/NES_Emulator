#include "ppuscroll.h"

int PpuScroll::GetAddr()
{
	int ret = addr[0] << 8 | addr[1];
	return ret;
}

void PpuScroll::Write(uint8_t value)
{
	auto loc = counter % 2;
	addr[loc] = value;
	counter++;
}

void PpuScroll::ClearCounter()
{
	counter = 0;
}

void PpuScroll::Clear()
{
	counter = 0;
	addr[0] = 0;
	addr[1] = 0;
}

void PpuScroll::Add(uint8_t val)
{
	int current_addr = GetAddr();
	current_addr += val;
	current_addr &= 0x3FFF;
	addr[0] = (current_addr >> 8) & 0xFF;
	addr[1] = current_addr & 0xFF;
}