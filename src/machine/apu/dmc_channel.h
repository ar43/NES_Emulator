#pragma once
class Memory;

class DmcChannel
{
public:
	bool dmcirq_enabled = false;
	bool dmc_loop = false;
	int freq_timer = 0;
	int output = 0;
	int addrload = 0;
	int lengthload = 0;
	bool dmcirq_pending = false;
	int length = 0;
	int addr = 0;

	


	void Clock(Memory *mem);
private:
	int freq_counter = 0;
	bool output_unit_silent = true;
	int output_shift = 0;
	int bits_in_output_unit = 0;
	int sample_buffer = 0;
	bool is_sample_buffer_empty = true;
};