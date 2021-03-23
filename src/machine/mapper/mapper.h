#pragma once
#include "../../logger/logger.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include <memory>
#include <cassert>

class Mapper
{
public:
	virtual void WritePRG(size_t loc, uint8_t byte) = 0;
	virtual uint8_t ReadPRG(size_t loc) = 0;
	virtual bool WritePPU(size_t loc, uint8_t byte) = 0;
	virtual int ReadPPU(size_t loc) = 0;
	virtual void ClearRegisters(){}
	virtual void SaveRAM(std::string name) {}
	virtual void LoadRAM(std::string name) { if (battery)logger::PrintLine(logger::LogType::WARNING, "ROM requests battery but it is not implemented in the mapper!"); }
	int getNumber();
	bool use_chr_ram = false;
	int nametable_mirroring; //2 vertical mirroring, 3 horizontal mirroring
	std::string name;
	bool battery = false;
protected:
	int number;

};