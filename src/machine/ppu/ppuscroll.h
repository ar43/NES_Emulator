#pragma once

#include <array>

class PpuScroll
{
public:
	PpuScroll()
	{
		addr[0] = 0;
		addr[1] = 0;
		counter = 0;
	}
	int GetAddr();
	void Write(uint8_t value);
	void Add(uint8_t val);
	void ClearCounter();
	void Clear();
	uint8_t addr[2];
	bool changed = false;
private:
	uint32_t counter;
	
};