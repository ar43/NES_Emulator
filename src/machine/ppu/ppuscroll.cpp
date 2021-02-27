#include "ppuscroll.h"
#include "../../logger/logger.h"

void PpuScroll::Write(uint8_t value)
{
	auto loc = *w % 2;
	addr[loc] = value;
	*w = *w + 1;
}

void PpuScroll::Clear()
{
	addr[0] = 0;
	addr[1] = 0;
}