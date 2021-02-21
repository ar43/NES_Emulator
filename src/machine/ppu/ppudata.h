#pragma once
#include <cstdint>

class PpuData
{
public:
	PpuData()
	{
		data = 0;
	}
	uint8_t Get();
	void Set(uint8_t data);
private:
	uint8_t data;
};