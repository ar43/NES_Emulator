#include "noise_channel.h"
#include "../../utility/utility.h"

void NoiseChannel::Clock()
{
	if (freq_counter > 0)
		freq_counter--;
	else
	{
		freq_counter = freq_timer;
		if (shift_mode)
			utility::SetBit(&noise_shift, 15, utility::IsBitSetInt(noise_shift,6) ^ utility::IsBitSetInt(noise_shift,0));
		else
			utility::SetBit(&noise_shift, 15, utility::IsBitSetInt(noise_shift,1) ^ utility::IsBitSetInt(noise_shift,0));
		noise_shift >>= 1;
	}

	if (!utility::IsBitSet(noise_shift, 0) && length_counter != 0)
	{
		if (decay_enabled)
			output = decay_hidden_vol;
		else
			output = decay_v;
	}
	else
	{
		output = 0;
	}
}

void NoiseChannel::ClockQuarter()
{
	if(decay_reset_flag)
	{
		decay_reset_flag = false;
		decay_hidden_vol = 0xF;
		decay_counter = decay_v;
	}
	else
	{
		if (decay_counter > 0)
			--decay_counter;
		else
		{
			decay_counter = decay_v;
			if (decay_hidden_vol > 0)
				--decay_hidden_vol;
			else if (decay_loop)
				decay_hidden_vol = 0xF;
		}
	}
}

void NoiseChannel::ClockHalf()
{
	if (length_enabled && length_counter > 0)
		length_counter--;
}