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
	pulse_channel[0].is_pulse1 = true;
	InitPulseTable();
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
	sample_timer = 20;
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
		pulse_channel[i].Think();
	}
	

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
		if (pulse_channel[0].len && pulse_channel[0].enable && !pulse_channel[0].IsMutedBySweep())
		{
			pulse1 = pulse_channel[0].freq;
		}
		if (pulse_channel[1].len && pulse_channel[1].enable && !pulse_channel[1].IsMutedBySweep())
		{
			pulse2 = pulse_channel[1].freq;
		}

		//filter
		static float HPA_Prev = 0;
		static float HPB_Prev = 0;
		static float LP_Out = 0;
		static float HPB_Out = 0;
		static float HPA_Out = 0;
		float sample = pulseTable[pulse1 + pulse2];
		float LP_In = sample;
		LP_Out = (LP_In - LP_Out) * 0.815686f;
		
		HPA_Out = HPA_Out * 0.996039f + LP_Out - HPA_Prev;
		HPA_Prev = LP_Out;

		HPB_Out = HPB_Out * 0.999835f + HPA_Out - HPB_Prev;
		HPB_Prev = HPA_Out;


		float output = HPB_Out;
		snd_buf.back() = output;
	}
}

void Apu::InitPulseTable()
{
	for (auto i = 0; i < 31; i++)
	{
		pulseTable[i] = 95.52f / (8128.0f / i + 100.0f);
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