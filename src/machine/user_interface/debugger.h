#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <array>
#include "window.h"
#include <set>

enum class Breakpoint
{
	INACTIVE,
	SUPRESSED,
	ACTIVE
};

enum class DebuggerSignal
{
	CLEAR,
	PAUSE,
	CONTINUE
};

class Register;

struct CpuData
{
	Register* registers[6];
	bool update = false;
};

struct DebugData
{
	DebugData()
	{
		Clear();
	}
	std::array<std::string, 0x10000> code;
	int known_bytes = 0;
	int all_bytes = 0x8000;
	bool mirror = false;
	std::set<int> breakpoints;
	Breakpoint breakpoint[0x10000];

	CpuData cpu_data;

	DebuggerSignal signal = DebuggerSignal::CLEAR;
	int step = 0;
	int force_cursor = 0;

	int breakpoint_hit = 0;
	int hit = 0;

	void Clear()
	{
		known_bytes = 0;
		for (int i = 0; i < 0x10000; i++)
		{
			code[i].clear();
			breakpoint[i] = Breakpoint::INACTIVE;
		}
		breakpoints.clear();
		signal = DebuggerSignal::CLEAR;
		step = 0;
		force_cursor = 0;
		breakpoint_hit = 0;
		hit = 0;
		mirror = false;
	}
};

class Text;
class Button;
struct MachineStatus;
class AsmList;

class Debugger
{
private:
	
	static const int win_width = 750;
	static const int win_height = 700;

	Text *text_status;
	Text* text_registers;
	Text* text_flags;
	Button* button_attach;
	Button* button_breakpoint_toggle;
	Button* button_continue;
	Button* button_step;
	Button* button_step_over;
	AsmList* asm_list;
	SDL_Window* window_main;

public:
	Window window;
	void Init(SDL_Window* window_main);
	void DrawBackground(SDL_Renderer* renderer);
	void Continue();
	void StepIn();
	void StepOver();
	void Button1Click();
	void ToggleBreakpoint();
	void Checkbox1Click(bool *new_state);
	void Open();
	void Close();
	void Update();
	void Attach();
	void Detach();
	void UpdateCpuData();
	

	DebugData debug_data;
	MachineStatus *machine_status;
	bool* debug_mode = nullptr;
	int mapper = -1;
};

