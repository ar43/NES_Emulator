#pragma once
#include <cstdint>

//7  bit  0
//---- ----
//VPHB SINN
//|||| ||||
//|||| ||++- Base nametable address
//|||| ||    (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
//|||| |+--- VRAM address increment per CPU read/write of PPUDATA
//|||| |     (0: add 1, going across; 1: add 32, going down)
//|||| +---- Sprite pattern table address for 8x8 sprites
//||||       (0: $0000; 1: $1000; ignored in 8x16 mode)
//|||+------ Background pattern table address (0: $0000; 1: $1000)
//||+------- Sprite size (0: 8x8 pixels; 1: 8x16 pixels)
//|+-------- PPU master/slave select
//|          (0: read backdrop from EXT pins; 1: output color on EXT pins)
//+--------- Generate an NMI at the start of the
//vertical blanking interval (0: off; 1: on)

enum class ControllerBits
{
	BASE_NAMETABLE1,
	BASE_NAMETABLE2,
	VRAM_INC,
	SPRITE_PATTERN, //sprite bank id
	BACKGROUND_PATTERN, //bg bank id
	SPRITE_SIZE, // 8-16 mode
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