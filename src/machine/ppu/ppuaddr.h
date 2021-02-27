#pragma once
#include <array>

class PpuAddr
{
public:
	PpuAddr()
	{
		addr[0] = 0;
		addr[1] = 0;
	}
	int GetAddr();
	void Write(uint8_t value, int *t, int *v);
	void Add(uint8_t val);
	void Clear();
	int* w;
private:
	std::array<uint8_t, 2> addr;
	
};