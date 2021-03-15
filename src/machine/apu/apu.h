#pragma once
#include <cstdint>
#include "frame_counter.h"
#include "pulse_channel.h"
#include "triangle_channel.h"
#include "noise_channel.h"
#include "dmc_channel.h"

class Bus;

class Apu
{
public:
	Apu();
	void Reset();
	void Init(bool* irq_pointer);
	void Step(Bus *bus, uint16_t budget);
	void Tick(Bus *bus);
	void Frame0Tick();
	void Frame1Tick();
	void InitSoundTables();
	void GenerateSample();
	void Play();
	void WriteRegisters(size_t loc, uint8_t byte);
	uint8_t ReadStatus();

	bool mute = false;

	FrameCounter frame_counter;
	PulseChannel pulse_channel[2];
	TriangleChannel triangle_channel;
	NoiseChannel noise_channel;
	DmcChannel dmc_channel;
	
	int cycles = 0;
private:
	bool canTick = false;
	
	int sample_timer = 20;
	
	float pulse_table[31];
	float tnd_table[203];

	bool* irq_pending = nullptr;
	std::vector<float> snd_buf;
};