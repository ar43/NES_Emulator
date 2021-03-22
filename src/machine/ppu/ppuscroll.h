#pragma once

#include <array>

class PpuScroll
{
public:
	PpuScroll()
	{
		addr[0] = 0;
		addr[1] = 0;
		addr[2] = 0;
		w = nullptr;
	}
	void Write(uint8_t value);
	void Clear();
	uint8_t addr[3];
	int* w;
	int* x;
	int* t;
private:
	
	
};