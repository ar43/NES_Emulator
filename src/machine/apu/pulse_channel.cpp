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

bool PulseChannel::IsMutedBySweep()
{
	if (timer_target < 8)
		return true;
	else if (!sweep_negate && timer_target + (timer_target >> sweep_shift) >= 0x800)
		return true;
	else
		return false;
}

void PulseChannel::ClockSL()
{
	
	if (!length_counter_halt &&len)
		len--;

	//sweep

	if (sweep_reload)
	{
		sweep_divider = sweep_period;
		sweep_reload = false;
	}
	else if (sweep_divider > 0)
	{
		sweep_divider--;
	}
	else
	{
		sweep_divider = sweep_period;
		if( sweep_enable && !IsMutedBySweep() )
		{
			if (sweep_negate)
			{
				timer_target -= (timer_target >> sweep_shift);
				if (is_pulse1)
					timer_target++;
			}
			else
			{
				timer_target += (timer_target >> sweep_shift);
			}
				
		}
	}
}