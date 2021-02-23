#pragma once
#include <cstdint>

enum class ControllerBits
{
	BASE_NAMETABLE1,
	BASE_NAMETABLE2,
	VRAM_INC,
	SPRITE_PATTERN, //bg bank id
	BACKGROUND_PATTERN, //bg bank id
	SPRITE_SIZE,
	PPU_MASTERSLAVE,
	GEN_NMI
};

class PpuCtrl
{
public:
	int GetNametable();
	bool IsBitSet(ControllerBits bit);
	void Set(uint8_t value);
private:
	uint8_t value = 0;
};