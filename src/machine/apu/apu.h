#pragma once
#include <cstdint>
#include "frame_counter.h"
#include "pulse_channel.h"
#include "triangle_channel.h"

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
	void InitSoundTables();
	void GenerateSample();

	bool mute = false;

	FrameCounter frame_counter;
	PulseChannel pulse_channel[2];
	TriangleChannel triangle_channel;
	
	

	std::vector<float> snd_buf;
	int cycles = 0;
private:
	bool canTick = false;
	
	int sample_timer = 20;
	
	float pulse_table[31];
	float tnd_table[203];
	
};