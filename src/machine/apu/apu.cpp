#include "apu.h"
#include <SDL.h>

void Apu::Init()
{
	SDL_setenv("SDL_AUDIODRIVER", "directsound", 1);
	SDL_InitSubSystem(SDL_INIT_AUDIO);
	SDL_AudioSpec AudioSettings = { 0 };

	AudioSettings.freq = 44100;
	AudioSettings.format = AUDIO_F32SYS;
	AudioSettings.channels = 2;
	AudioSettings.samples = 1024;

	SDL_OpenAudio(&AudioSettings, 0);
}