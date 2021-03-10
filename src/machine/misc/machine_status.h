#pragma once

struct MachineStatus
{
	bool running = true;
	bool paused = false;
	bool* speedup = nullptr;
	bool* mute = nullptr;
	int reset = 2;
};