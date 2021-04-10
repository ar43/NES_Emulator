#pragma once
#include <string>

enum class RunningStatus
{
	NOT_RUNNING,
	RUNNING,
	RUNNING_ROM
};

enum class ResetType
{
	NOT,
	NORMAL,
	HARD
};


struct MachineStatus
{
	RunningStatus running = RunningStatus::RUNNING;
	std::string pending_rom;
	bool paused = false;
	bool* speedup = nullptr;
	bool* mute = nullptr;
	ResetType reset = ResetType::HARD;
	bool force_render = false;
	float volume = 0.2f;
};