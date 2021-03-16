#pragma once
#include <array>

class PpuAddr
{
public:
	PpuAddr()
	{
		addr[0] = 0;
		addr[1] = 0;
		w = nullptr;
	}
	int GetAddr();
	void Write(uint8_t value, int *t, int *v);
	void Add(uint8_t val);
	void Clear();
	int* w;
	int* x;
	uint8_t* scrolladdr;
private:
	uint8_t addr[2];
	
};