#pragma once
#include "ppuaddr.h"
#include "ppuctrl.h"
#include "ppudata.h"
#include "ppumask.h"
#include "ppuscroll.h"
#include "ppustatus.h"

struct PpuRegisters
{
	PpuCtrl ppuctrl;
	PpuMask ppumask;
	PpuStatus ppustatus;
	uint8_t oamaddr = 0;
	uint8_t oamdata;
	PpuScroll ppuscroll;
	PpuAddr ppuaddr;
	PpuData ppudata;
	uint8_t oamdma;
	int t = 0;
	int v = 0;
	int w = 0;
	int x = 0;
};