#pragma once
#include <cstdint>
#include <vector>
#include <memory>

constexpr size_t INES_PRG_BLOCK_SIZE = 16384;
constexpr size_t INES_CHR_BLOCK_SIZE = 8192;

enum class Flags6
{
	MIRRORING,
	BATTERY,
	TRAINER,
	FOUR_SCREEN_VRAM
};

enum class Flags7
{
	VS_UNISYSTEM,
	PLAY_CHOICE_10
};

struct NesHeader
{
	uint8_t PRG_ROM_size; //16384 * x
	uint8_t CHR_ROM_size; //8192 * y
	uint8_t flags6;
	uint8_t flags7;
	uint8_t flags8; //PRG_RAM size in 8KB units
	uint8_t flags9; //0 NTSC 1 PAL
	uint8_t flags10; //unofficial?
	int mapper_num;
	bool nes2 = false;
};

struct NesData
{
	NesHeader header;
	std::array<char, 512> trainer_data; //kinda useless
	std::vector<std::shared_ptr<char>> prg_rom;
	std::vector<std::shared_ptr<char>> chr_rom;
	std::string file_name = "empty";
	//std::vector<std::unique_ptr<uint8_t>> playchoice_instrom; this is useless
	//std::vector<std::unique_ptr<uint8_t>> playchoice_prom;   this is useless
};