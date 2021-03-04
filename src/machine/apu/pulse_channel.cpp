#include "pulse_channel.h"

void PulseChannel::ClockEnv()
{
	if (!env_start)
	{
		envelope_div--;
		if (envelope_div < 0)
		{
			envelope_div = envelope_divider;
			if (envelope > 0)
			{
				envelope--;
				envelopeVol = envelope;
			}
			else
			{
				envelopeVol = envelope;
				if (length_counter_halt)
					envelope = 15;
			}
		}
	}
	else
	{
		env_start = false;
		envelope = 15;
		envelope_div = envelope_divider;
	}
}

void PulseChannel::ClockSL()
{
	if (!length_counter_halt)
	{
		if (len)
			len--;
	}
	//sweep
}