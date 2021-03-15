#include "dmc_channel.h"
#include "../bus/bus.h"


void DmcChannel::Clock(Bus *bus)
{
	if (freq_counter)
	{
		freq_counter--;
	}
	else
	{
		freq_counter = freq_timer;

		if (!output_unit_silent)
		{
			if ((output_shift & 1) && output < 0x7E)
				output += 2;
			if (!(output_shift & 1) && output > 0x01)
				output -= 2;
		}
		bits_in_output_unit--;
		output_shift >>= 1;

		if (bits_in_output_unit <= 0)
		{
			bits_in_output_unit = 8;
			output_shift = sample_buffer;
			output_unit_silent = is_sample_buffer_empty;
			is_sample_buffer_empty = true;
		}
	}

	if (length > 0 && is_sample_buffer_empty)
	{
		sample_buffer = bus->ReadCPUSafe(addr);
		is_sample_buffer_empty = false;
		addr++;
		if (addr > 0xFFFF)
			addr = 0x8000;
		length--;
		if (length == 0)
		{
			if (dmc_loop)
			{
				length = lengthload;
				addr = addrload;
			}
			else if (dmcirq_enabled)
			{
				dmcirq_pending = true;
			}
		}
	}
}