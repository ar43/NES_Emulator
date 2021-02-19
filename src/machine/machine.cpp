#include "machine.h"
#include "../logger/logger.h"
#include <fstream>
#include <memory>

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)

Machine::Machine()
{
	logger::PrintLine(logger::LogType::INFO, "Created machine");
}

bool Machine::LoadNES(std::string path)
{
	std::size_t found = path.find(".nes");
	if (found == std::string::npos)
	{
		logger::PrintLine(logger::LogType::ERROR, "Bad file format! .nes required");
		return false;
	}

	std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);
	if (!ifs.is_open())
	{
		logger::PrintLine(logger::LogType::ERROR, "File does not exist!");
		return false;
	}
	char header[16];
	ifs.read(header, 16);
	if (!ifs)
	{
		logger::PrintLine(logger::LogType::ERROR, "Bad file format! File too short.");
		ifs.close();
		return false;
	}
	if (strncmp(header, "NES\x1A", 4) != 0)
	{
		logger::PrintLine(logger::LogType::ERROR, "Bad file signature!");
		ifs.close();
		return false;
	}
	nes_data = std::unique_ptr<NesData>(new NesData());
	nes_data->header.PRG_ROM_size = (uint8_t)header[4];
	nes_data->header.CHR_ROM_size = (uint8_t)header[5];
	nes_data->header.flags6 = (uint8_t)header[6];
	nes_data->header.flags7 = (uint8_t)header[7];
	nes_data->header.flags8 = (uint8_t)header[8];
	nes_data->header.flags9 = (uint8_t)header[9];
	nes_data->header.flags10 = (uint8_t)header[10];
	if ((nes_data->header.flags7 & 24) == 24)
	{
		nes_data->header.nes2 = true;
	}

	nes_data->header.mapper_num = HI_NIBBLE(nes_data->header.flags7) * 16 + HI_NIBBLE(nes_data->header.flags6);
	assert(nes_data->header.mapper_num >= 0 && nes_data->header.mapper_num <= 255);

	for (int i = 0; i < nes_data->header.PRG_ROM_size;i++)
	{
		std::shared_ptr<char> data(new char[INES_PRG_BLOCK_SIZE]);
		ifs.read(data.get(), INES_PRG_BLOCK_SIZE);
		if (!ifs)
		{
			logger::PrintLine(logger::LogType::ERROR, "Bad file format! File too short.");
			ifs.close();
			return false;
		}
		nes_data->prg_rom.push_back(data);
	}
	for (int i = 0; i < nes_data->header.CHR_ROM_size;i++)
	{
		std::shared_ptr<char> data(new char[INES_CHR_BLOCK_SIZE]);
		ifs.read(data.get(), INES_CHR_BLOCK_SIZE);
		if (!ifs)
		{
			logger::PrintLine(logger::LogType::ERROR, "Bad file format! File too short.");
			ifs.close();
			return false;
		}
		nes_data->chr_rom.push_back(data);
	}

	ifs.close();
	return true;
}