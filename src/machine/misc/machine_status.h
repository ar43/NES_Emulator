#pragma once

struct MachineStatus
{
	bool running = true;
	bool paused = false;
	bool* speedup;
	bool* mute;
	int reset = 2;
};