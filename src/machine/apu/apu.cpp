#include "apu.h"
#include <SDL.h>
#include <string>
#include "../../logger/logger.h"
#include "../memory/memory.h"

Apu::Apu()
{
	Reset();
}

void Apu::Init()
{
	SDL_setenv("SDL_AUDIODRIVER", "directsound", 1);
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	SDL_AudioSpec AudioSettings = {0};

	//// NOTE: Sound test
	//int SamplesPerSecond = 44100;
	//int ToneHz = 440;
	//float ToneVolume = 0.002f;
	//uint32_t RunningSampleIndex = 0;
	//int SquareWavePeriod = SamplesPerSecond / ToneHz;
	//int HalfSquareWavePeriod = SquareWavePeriod / 2;
	//int BytesPerSample = sizeof(float);

	AudioSettings.freq = 44100;
	AudioSettings.format = AUDIO_F32;
	AudioSettings.channels = 1;
	AudioSettings.samples = 1024;

	SDL_OpenAudio(&AudioSettings, 0);
	SDL_PauseAudio(0);
	//int BytesToWrite = 735*BytesPerSample*60; //in one frame we need to play 735 samples. we need to take every 40th sample to achieve that
	//
	//void *SoundBuffer = malloc(BytesToWrite);
	//float *SampleOut = (float *)SoundBuffer;
	//int SampleCount = BytesToWrite/BytesPerSample;

	//for(int SampleIndex = 0; SampleIndex < SampleCount; ++SampleIndex)
	//{
	//	float SampleValue = ((RunningSampleIndex++ / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
	//	*SampleOut++ = SampleValue;
	//}

	//SDL_QueueAudio(1, SoundBuffer, BytesToWrite);
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
}

void Apu::Tick(Memory *mem)
{
	if (!canTick)
	{
		canTick = true;
		return;
	}
	canTick = false;
	cycles++;

	if (!frame_counter.mode)
		Frame0Tick(mem);
	else
		Frame1Tick();

	for (int i = 0; i < 2; i++)
	{
		if (pulse_channel[i].c)
			pulse_channel[i]._amp = pulse_channel[i].envelope_divider;
		else
			pulse_channel[i]._amp = pulse_channel[i].envelope_vol;

		//handle timer
		if (pulse_channel[i].timer <= 0) 
		{
			pulse_channel[i].timer = pulse_channel[i].timer_target;

			//tick duty pointer
			++pulse_channel[i].duty_index %= 8;
		}
		else 
		{
			pulse_channel[i].timer--;
		}

		//	handle duty
		int duty = pulse_channel[i].duty_cycle;
		if (APU_DUTY_TABLE[duty][pulse_channel[i].duty_index] == 1)
			pulse_channel[i].freq = pulse_channel[i]._amp;
		else
			pulse_channel[i].freq = 0;
	}
	

	if (sample_timer == 0)
	{
		sample_timer = 20;

		snd_buf.push_back(0);
		float pulse1 = 0;
		float pulse2 = 0;
		if (pulse_channel[0].len && pulse_channel[0].enable)
		{
			pulse1 = (float)pulse_channel[0].freq;
		}
		if (pulse_channel[1].len && pulse_channel[1].enable)
		{
			pulse2 = (float)pulse_channel[1].freq;
		}
		snd_buf.back() = 0.00752f * (pulse1 + pulse2);
	}
	else
	{
		sample_timer--;
	}
	
}

void Apu::Frame0Tick(Memory *mem)
{
	if (cycles == 3729 || cycles == 11186)
	{
		pulse_channel[0].ClockEnv();
		pulse_channel[1].ClockEnv();
	}
	else if (cycles == 7457 || cycles == 14915)
	{
		pulse_channel[0].ClockEnv();
		pulse_channel[0].ClockSL();
		pulse_channel[1].ClockEnv();
		pulse_channel[1].ClockSL();
	}
	if (cycles >= 14915)
	{
		cycles = 0;
		if (!frame_counter.interrupt_inhibit)
			mem->trigger_irq_interrupt = true;
	}
}

void Apu::Frame1Tick()
{
	if (cycles == 3729 || cycles == 11186)
	{
		pulse_channel[0].ClockEnv();
		pulse_channel[1].ClockEnv();
	}
	else if (cycles == 7457 || cycles == 18641)
	{
		pulse_channel[0].ClockEnv();
		pulse_channel[0].ClockSL();
		pulse_channel[1].ClockEnv();
		pulse_channel[1].ClockSL();
	}
	if (cycles >= 18641)
	{
		cycles = 0;
	}
}