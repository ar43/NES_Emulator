#pragma once
#include <cstdint>
#include <string>

constexpr size_t CPU_MEM_SIZE = 0x10000;
constexpr size_t PPU_MEM_SIZE = 0x4000;
constexpr size_t OAM_MEM_SIZE = 0x100;
constexpr size_t CHR_ROM_SIZE = 0x2000;

constexpr uint32_t SCREEN_WIDTH = 256;
constexpr uint32_t SCREEN_HEIGHT = 240;
constexpr int PIXEL_PER_TILE = 64;
constexpr int TILE_WIDTH = 8;
constexpr int TILE_HEIGHT = 8;
constexpr int TILE_PER_BANK = 256;
constexpr int BYTES_PER_TILE = 16;
constexpr int NUM_SPRITES = 64;
constexpr int BYTES_PER_SPRITE = 4;
constexpr int TILES_X = 32;
constexpr int TILES_Y = 30;

constexpr int APU_LEN_TABLE[] = { 10,254, 20,  2, 40,  4, 80,  6, 160,  8, 60, 10, 14, 12, 26, 14,12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30 };
constexpr uint8_t APU_DUTY_TABLE[4][8] = {
	{0, 0, 0, 0, 0, 0, 0, 1 },
	{0, 0, 0, 0, 0, 0, 1, 1 },
	{0, 0, 0, 0, 1, 1, 1, 1 },
	{1, 1, 1, 1, 1, 1, 0, 0 }
};
constexpr int APU_NOISE_FREQ_TABLE[] = { 4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068 };
constexpr int APU_DMC_FREQ_TABLE[] = { 428, 380, 340, 320, 286, 254, 226, 214, 190, 160, 142, 128, 106,  84,  72,  54 };
constexpr size_t PALETTE_SIZE = 192;

constexpr auto BATTERY_PATH = "data/battery";
constexpr auto DEBUG_PATH = "data/debugger";
constexpr auto FONT_PATH = "data/ui/Consolas.ttf";
constexpr auto CHECKMARK_PATH = "data/ui/checkmark2.png";
constexpr auto CONFIG_PATH = "data/config.ini";