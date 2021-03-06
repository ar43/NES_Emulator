#include "pulse_channel.h"
#include "../misc/constants.h"

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
				envelope_vol = envelope;
			}
			else
			{
				envelope_vol = envelope;
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

void PulseChannel::Think()
{
	if (c)
		_amp = envelope_divider;
	else
		_amp = envelope_vol;

	if (timer <= 0) 
	{
		timer = timer_target;

		duty_index = (duty_index + 1) & 7;
	}
	else 
	{
		timer--;
	}

	if (APU_DUTY_TABLE[duty_cycle][duty_index] == 1)
		freq = _amp;
	else
		freq = 0;
}

void PulseChannel::ClockSL()
{
	
	if (!length_counter_halt &&len)
		len--;

	//sweep
	if (sweep_enable)
	{
		sweep_divider--;
		if (sweep_divider < 0)
		{
			int post = 0; 
			if (sweep_negate)
			{
				post -= timer_target >> sweep_shift;
				if (is_pulse1)
					post++;
			}
			else
			{
				post += timer_target >> sweep_shift;
			}

			if (post >= 0x7ff && !sweep_negate || timer_target <= 8)
			{
				//_amp = 0;
				//sweep_enable = false;
				muted_by_sweep = true;
			}
			else
			{
				muted_by_sweep = false;
				timer_target += post;
			}

		}
		if (sweep_divider < 0 || sweep_reload)
		{
			sweep_reload = false;
			sweep_divider = sweep_period;
		}
	}
}