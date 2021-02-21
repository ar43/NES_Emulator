#pragma once
#include <cstdint>

enum class StatusBits
{
	BIT0,
	BIT1,
	BIT2,
	BIT3,
	BIT4,
	SPRITE_OVERFLOW,
	SPRITE0_HIT,
	VBLANK
};

class PpuStatus
{
public:
	bool IsBitSet(StatusBits bit);
	void Set(uint8_t value);
	uint8_t Get();
	void ResetVblank();
private:
	uint8_t value = 0;
};