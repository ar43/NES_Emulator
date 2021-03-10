#pragma once
#include <cstdint>

class NoiseChannel
{
public:
	NoiseChannel() 
	{
		Init();
	}
	void Init() 
	{
		enable = false;
		length_counter = 0;

		decay_loop = false;
		length_enabled = false;
		decay_enabled = false;
		decay_v = 0;
		decay_reset_flag = false;
		decay_hidden_vol = 0;
		decay_counter = 0;

		freq_timer = 0;
		shift_mode = false;
		noise_shift = 1;
		freq_counter = 0;

		output = 0;
	}
	void Clock();
	void ClockQuarter();
	void ClockHalf();

	bool enable;
	int length_counter;

	bool decay_loop;
	bool length_enabled;
	bool decay_enabled;
	bool decay_reset_flag;
	int decay_v;
	int decay_hidden_vol;
	int decay_counter;

	int freq_timer;
	int freq_counter;
	bool shift_mode;
	uint32_t noise_shift;

	int output;
};