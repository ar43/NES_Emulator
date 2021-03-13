#include "apu.h"
#include <SDL.h>
#include <string>
#include "../../logger/logger.h"
#include "../memory/memory.h"

Apu::Apu()
{
	
}

void Apu::Init(bool* irq_pointer)
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
	Reset();
}

void Apu::Step(Memory *mem, uint16_t budget)
{
	for (auto i = 0; i < budget; i++)
	{
		Tick(mem);
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

void Apu::Tick(Memory *mem)
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
	dmc_channel.Clock(mem);

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


		float output = HPB_Out;
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