#include "ppudata.h"

void PpuData::Set(uint8_t data)
{
	this->data = data;
}

uint8_t PpuData::Get()
{
	return this->data;
}