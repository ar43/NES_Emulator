#pragma once

struct MachineStatus
{
	bool running = true;
	bool paused = false;
	bool* speedup = nullptr;
	bool* mute = nullptr;
	int reset = 2;
	bool force_render = false;
	float volume = 0.2f;
};