#include "apu.h"
#include <SDL.h>
#include <string>
#include "../../logger/logger.h"
#include "../bus/bus.h"
#include "../../utility/utility.h"

Apu::Apu()
{
	
}

void Apu::WriteRegisters(size_t loc, uint8_t byte)
{

	switch (loc)
	{
		//pulse 1
		case 0x4000:
		{
			pulse_channel[0].duty_cycle = (byte >> 6) & 3;
			pulse_channel[0].length_counter_halt = utility::IsBitSet(byte, 5);
			pulse_channel[0].c = utility::IsBitSet(byte, 4);
			pulse_channel[0].envelope_divider = byte & 15;
			break;
		}
		case 0x4001:
		{
			pulse_channel[0].sweep_enable = utility::IsBitSet(byte, 7);
			pulse_channel[0].sweep_negate = utility::IsBitSet(byte, 3);
			pulse_channel[0].sweep_period = (byte >> 4) & 7;
			pulse_channel[0].sweep_shift = byte & 7;
			pulse_channel[0].sweep_reload = true;
			break;
		}
		case 0x4002:
		{
			const int prev = pulse_channel[0].timer_target & 0xFF00; 
			pulse_channel[0].timer_target = prev | byte;
			break;
		}
		case 0x4003:
		{
			const int prev = pulse_channel[0].timer_target & 0x00FF; 
			pulse_channel[0].timer_target = prev | ((byte & 0x07) << 8);
			if(pulse_channel[0].enable)
				pulse_channel[0].len = APU_LEN_TABLE[(byte >> 3) & 0x1f];
			pulse_channel[0].timer = pulse_channel[0].timer_target;
			pulse_channel[0].env_start = true;
			pulse_channel[0].duty_index = 0;
			break;
		}
		//pulse 2
		case 0x4004:
		{
			pulse_channel[1].duty_cycle = (byte >> 6) & 3;
			pulse_channel[1].length_counter_halt = utility::IsBitSet(byte, 5);
			pulse_channel[1].c = utility::IsBitSet(byte, 4);
			pulse_channel[1].envelope_divider = byte & 15;
			break;
		}
		case 0x4005:
		{
			pulse_channel[1].sweep_enable = utility::IsBitSet(byte, 7);
			pulse_channel[1].sweep_negate = utility::IsBitSet(byte, 3);
			pulse_channel[1].sweep_period = (byte >> 4) & 7;
			pulse_channel[1].sweep_shift = byte & 7;
			pulse_channel[1].sweep_reload = true;
			break;
		}
		case 0x4006:
		{
			const int prev = pulse_channel[1].timer_target & 0xFF00; 
			pulse_channel[1].timer_target = prev | byte;
			break;
		}
		case 0x4007:
		{
			const int prev = pulse_channel[1].timer_target & 0x00FF; 
			pulse_channel[1].timer_target = prev | ((byte & 0x07) << 8);
			if(pulse_channel[1].enable)
				pulse_channel[1].len = APU_LEN_TABLE[(byte >> 3) & 0x1f];
			pulse_channel[1].timer = pulse_channel[1].timer_target;
			pulse_channel[1].env_start = true;
			pulse_channel[1].duty_index = 0;
			break;
		}
		//triangle
		case 0x4008:
		{
			triangle_channel.linear_control = utility::IsBitSet(byte, 7);
			triangle_channel.length_enabled = !triangle_channel.linear_control;
			triangle_channel.linear_load = byte & 0x7F;
			break;
		}
		case 0x4009:
		{
			//unused
			break;
		}
		case 0x400A:
		{
			const int prev = triangle_channel.freq_timer & 0xFF00;
			triangle_channel.freq_timer = prev | byte;
			break;
		}
		case 0x400B:
		{
			const int prev = triangle_channel.freq_timer & 0x00FF;
			triangle_channel.freq_timer = prev | ((byte & 0x07) << 8);
			if (triangle_channel.enable)
				triangle_channel.length_counter = APU_LEN_TABLE[(byte >> 3) & 0x1f];
			triangle_channel.linear_reload = true;
			break;
		}
		//noise channel
		case 0x400C:
		{
			noise_channel.decay_loop = utility::IsBitSet(byte, 5);
			noise_channel.length_enabled = !utility::IsBitSet(byte, 5);
			noise_channel.decay_enabled = !utility::IsBitSet(byte, 4);
			noise_channel.decay_v = byte & 0x0F;
			break;
		}
		case 0x400D:
		{
			//unused
			break;
		}
		case 0x400E:
		{
			noise_channel.freq_timer = APU_NOISE_FREQ_TABLE[byte & 0x0F];
			noise_channel.shift_mode = utility::IsBitSet(byte, 7);
			break;
		}
		case 0x400F:
		{
			if(noise_channel.enable)
				noise_channel.length_counter = APU_LEN_TABLE[(byte >> 3) & 0x1f];
			noise_channel.decay_reset_flag = true;
			break;
		}
		//dmc channel
		case 0x4010:
		{
			dmc_channel.dmcirq_enabled = utility::IsBitSet(byte, 7);
			dmc_channel.dmc_loop = utility::IsBitSet(byte, 6);
			dmc_channel.freq_timer = APU_DMC_FREQ_TABLE[byte & 15] / 2; //CPU cycles to APU cycles
			if (!dmc_channel.dmcirq_enabled)
				dmc_channel.dmcirq_pending = false;
			break;
		}
		case 0x4011:
		{
			dmc_channel.output = byte & 0x7F;
			break;
		}
		case 0x4012:
		{
			dmc_channel.addrload = 0xC000 | (byte << 6);
			break;
		}
		case 0x4013:
		{
			dmc_channel.lengthload = (byte << 4) + 1;
			break;
		}
		//misc apu
		case 0x4015:
		{
			const bool pulse1_enable = utility::IsBitSet(byte, 0);
			const bool pulse2_enable = utility::IsBitSet(byte, 1);
			const bool triangle_enable = utility::IsBitSet(byte, 2);
			const bool noise_enable = utility::IsBitSet(byte, 3);
			const bool dmc_enable = utility::IsBitSet(byte, 4);
			if (!pulse1_enable)
			{
				pulse_channel[0].enable = false;
				pulse_channel[0].len = 0;
			}
			else
			{
				pulse_channel[0].enable = true;
			}

			if (!pulse2_enable)
			{
				pulse_channel[1].enable = false;
				pulse_channel[1].len = 0;
			}
			else
			{
				pulse_channel[1].enable = true;
			}

			if (!triangle_enable)
			{
				triangle_channel.enable = false;
				triangle_channel.length_counter = 0;
			}
			else
			{
				triangle_channel.enable = true;
			}

			if (!noise_enable)
			{
				noise_channel.enable = false;
				noise_channel.length_counter = 0;
			}
			else
			{
				noise_channel.enable = true;
			}
			if (!dmc_enable)
			{
				dmc_channel.length = 0;
			}
			else
			{
				if (dmc_channel.length == 0)
				{
					dmc_channel.length = dmc_channel.lengthload;
					dmc_channel.addr = dmc_channel.addrload;
				}
			}
			dmc_channel.dmcirq_pending = false;
			break;
		}
		case 0x4017:
		{
			frame_counter.interrupt_inhibit = utility::IsBitSet(byte, 6);
			frame_counter.mode = utility::IsBitSet(byte, 7);
			if (frame_counter.mode)
			{
				frame_counter.ClockQuarter(pulse_channel,&triangle_channel, &noise_channel);
				frame_counter.ClockHalf(pulse_channel,&triangle_channel, &noise_channel);
			}
			cycles = -1;
			break;
		}
		default:
		{
			logger::PrintLine(logger::LogType::FATAL_ERROR, "Apu::WriteRegisters - Bad APU addr");
			break;
		}
	}
}

uint8_t Apu::ReadStatus()
{
	uint8_t ret = 0;
	if (pulse_channel[0].len > 0)
	{
		ret = ret | 1;
	}
	if (pulse_channel[1].len > 0)
	{
		ret = ret | (1 << 1);
	}
	if (triangle_channel.length_counter > 0)
	{
		ret = ret | (1 << 2);
	}
	if (noise_channel.length_counter > 0)
	{
		ret = ret | (1 << 3);
	}
	if (dmc_channel.length > 0)
	{
		ret = ret | (1 << 4);
	}
	if (*irq_pending)
	{
		ret = ret | 1 << 6;
	}
	if (dmc_channel.dmcirq_pending)
	{
		ret = ret | 1 << 7;
	}
	*irq_pending = false;
	return ret;
}

void Apu::Init(bool* irq_pointer, float* volume)
{
	SDL_setenv("SDL_AUDIODRIVER", "directsound", 1);
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	SDL_AudioSpec AudioSettings = {0};

	AudioSettings.freq = 44100;
	AudioSettings.format = AUDIO_F32;
	AudioSettings.channels = 1;
	AudioSettings.samples = 1024;

	SDL_OpenAudio(&AudioSettings, 0);
	SDL_PauseAudio(0);
	pulse_channel[0].is_pulse1 = true;
	InitSoundTables();
	irq_pending = irq_pointer;
	this->volume = volume;
	Reset();
}

void Apu::Step(Bus *bus, uint16_t budget)
{
	for (auto i = 0; i < budget; i++)
	{
		Tick(bus);
	}
}

void Apu::Reset()
{
	canTick = false;
	sample_timer = 20;
	noise_channel.Init();
	//force re-sync
	snd_buf.clear();
	SDL_ClearQueuedAudio(1);
}

void Apu::Play()
{
	SDL_QueueAudio(1, snd_buf.data(), snd_buf.size() * sizeof(float));
	snd_buf.clear();
}

void Apu::Tick(Bus *bus)
{
	if (!canTick)
	{
		canTick = true;
		triangle_channel.Clock(); //clocks even on non-APU times
		return;
	}
	canTick = false;
	cycles++;

	if (!frame_counter.mode)
		Frame0Tick();
	else
		Frame1Tick();

	for (int i = 0; i < 2; i++)
	{
		pulse_channel[i].Clock();
	}
	triangle_channel.Clock();
	noise_channel.Clock();
	dmc_channel.Clock(bus);

	if (sample_timer == 0)
	{
		sample_timer = 20;
		GenerateSample();
	}
	else
	{
		sample_timer--;
	}
	
}

void Apu::GenerateSample()
{
	snd_buf.push_back(0);
	if (!mute)
	{
		int pulse1 = 0;
		int pulse2 = 0;
		int triangle = 0;
		int noise = 0;
		int dmc = dmc_channel.output;
		if (pulse_channel[0].len && pulse_channel[0].enable && !pulse_channel[0].IsMutedBySweep())
		{
			pulse1 = pulse_channel[0].freq;
		}
		if (pulse_channel[1].len && pulse_channel[1].enable && !pulse_channel[1].IsMutedBySweep())
		{
			pulse2 = pulse_channel[1].freq;
		}
		if (triangle_channel.enable) //possibly need to check for length_counter > 0 ? idk it sounds good anyways, lookout for bugs
		{
			triangle = triangle_channel.output;
		}
		if (noise_channel.length_counter && noise_channel.enable)
		{
			noise = noise_channel.output;
		}
		

		//filter
		static float HPA_Prev = 0;
		static float HPB_Prev = 0;
		static float LP_Out = 0;
		static float HPB_Out = 0;
		static float HPA_Out = 0;
		float pulse_out = pulse_table[pulse1 + pulse2];
		float tnd_out = tnd_table[triangle * 3 + noise * 2 + dmc];
		float LP_In = pulse_out + tnd_out;
		LP_Out = (LP_In - LP_Out) * 0.815686f;
		
		HPA_Out = HPA_Out * 0.996039f + LP_Out - HPA_Prev;
		HPA_Prev = LP_Out;

		HPB_Out = HPB_Out * 0.999835f + HPA_Out - HPB_Prev;
		HPB_Prev = HPA_Out;


		float output = HPB_Out * *volume;
		snd_buf.back() = output;
	}
}

void Apu::InitSoundTables()
{
	for (auto i = 0; i < 31; i++)
	{
		pulse_table[i] = 95.52f / (8128.0f / i + 100.0f);
	}
	for (auto i = 0; i < 203; i++)
	{
		tnd_table[i] = 163.67f / (24329.0f / i + 100.0f);
	}
}

void Apu::Frame0Tick()
{
	if (cycles == 3729 || cycles == 11186)
	{
		frame_counter.ClockQuarter(pulse_channel,&triangle_channel, &noise_channel);
	}
	else if (cycles == 7457 || cycles == 14915)
	{
		frame_counter.ClockQuarter(pulse_channel,&triangle_channel, &noise_channel);
		frame_counter.ClockHalf(pulse_channel,&triangle_channel, &noise_channel);
	}
	if (cycles >= 14915)
	{
		cycles = 0;
		if (!frame_counter.interrupt_inhibit)
			*irq_pending = true;
	}
}

void Apu::Frame1Tick()
{
	if (cycles == 3729 || cycles == 11186)
	{
		frame_counter.ClockQuarter(pulse_channel,&triangle_channel, &noise_channel);
	}
	else if (cycles == 7457 || cycles == 18641)
	{
		frame_counter.ClockQuarter(pulse_channel,&triangle_channel, &noise_channel);
		frame_counter.ClockHalf(pulse_channel,&triangle_channel, &noise_channel);
	}
	if (cycles >= 18641)
	{
		cycles = 0;
	}
}