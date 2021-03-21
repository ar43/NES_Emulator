#pragma once
#include "../../logger/logger.h"

class Mapper
{
public:
	virtual void WritePRG(size_t loc, uint8_t byte) = 0;
	virtual uint8_t ReadPRG(size_t loc) = 0;
	virtual bool WritePPU(size_t loc, uint8_t byte) = 0;
	virtual int ReadPPU(size_t loc) = 0;
	void ClearRegisters(){}
	int getNumber();
	bool use_chr_ram = false;
	int nametable_mirroring; //2 vertical mirroring, 3 horizontal mirroring
	std::string name;
protected:
	int number;

};