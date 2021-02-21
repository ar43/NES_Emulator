#pragma once
#include <cstdint>

enum class MaskBits
{
	GREYSCALE,
	SHOW_BACKGROUND_LEFT,
	SHOW_SPRITES_LEFT,
	SHOW_BACKGROUND,
	SHOW_SPRITES,
	EMPHASIZE_RED,
	EMPHASIZE_GREEN,
	EMPHASIZE_BLUE
};

class PpuMask
{
public:
	bool IsBitSet(MaskBits bit);
	void Set(uint8_t value);
private:
	uint8_t value = 0;
};