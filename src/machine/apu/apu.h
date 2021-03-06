#pragma once
#include <cstdint>
#include "frame_counter.h"
#include "pulse_channel.h"

class Memory;

class Apu
{
public:
	Apu();
	void Reset();
	void Init();
	void Step(Memory *mem, uint16_t budget);
	void Tick(Memory *mem);
	void Frame0Tick(Memory *mem);
	void Frame1Tick();

	FrameCounter frame_counter;
	PulseChannel pulse_channel[2];

	std::vector<float> snd_buf;
private:
	bool canTick = false;
	int cycles = 0;
	int sample_timer = 20;
	
};