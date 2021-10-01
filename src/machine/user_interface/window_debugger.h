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
	uint64_t cycles;
	bool update = false;
};

struct DebugData
{
	DebugData()
	{
		Clear();
	}
	std::array<std::string, 0x10000> code;
	std::array<bool, 0x10000> is_subroutine;
	int known_bytes = 0;
	int all_bytes = 0x8000;
	bool mirror = false;

	std::set<int> breakpoints;
	Breakpoint breakpoint[0x10000];
	std::vector<std::string> breakpoints_draw;

	CpuData cpu_data;

	DebuggerSignal signal = DebuggerSignal::CLEAR;
	int step = 0;
	int force_cursor = 0;

	int breakpoint_hit = 0;
	int hit = 0;

	std::string hash;

	void Clear()
	{
		known_bytes = 0;
		for (int i = 0; i < 0x10000; i++)
		{
			code[i].clear();
			breakpoint[i] = Breakpoint::INACTIVE;
			is_subroutine[i] = false;
		}
		breakpoints.clear();
		signal = DebuggerSignal::CLEAR;
		step = 0;
		force_cursor = 0;
		breakpoint_hit = 0;
		hit = 0;
		mirror = false;
		UpdateBreakpoints();
	}

	void UpdateBreakpoints();
};

class Text;
class Button;
struct MachineStatus;
class AsmList;

class WindowDebugger
{
private:
	
	static const int win_width = 750;
	static const int win_height = 500;

	Text *text_status;
	Text* text_registers;
	Text* text_flags;
	Text* text_cycles;

	Button* button_attach;
	Button* button_breakpoint_toggle;
	Button* button_continue;
	Button* button_step;
	Button* button_step_over;
	Button* button_goto;
	Button* button_remove_bp;
	Button* button_bp;
	
	Textbox* textbox_goto;
	Textbox* textbox_bp;
	AsmList* asm_list;
	List* list_breakpoints;
	SDL_Window* window_main;

public:
	Window window;
	
	void Init(SDL_Window* window_main);
	void RemoveBreakpoint();
	void Goto();
	void ToggleBreakpointText();
	void DrawBackground(SDL_Renderer* renderer);
	void Continue();
	void StepIn();
	void StepOver();
	void Button1Click();
	void ToggleBreakpoint(int loc = 0);
	void Checkbox1Click(bool *new_state);
	void Open();
	void Close();
	void Update();
	void Attach();
	void SaveData();
	void LoadData();
	void Detach();
	void UpdateCpuData();
	

	DebugData debug_data;
	MachineStatus *machine_status;
	bool* debug_mode = nullptr;
	int mapper = -1;
	bool enable_save_load = true;
};

