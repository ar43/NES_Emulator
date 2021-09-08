#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <array>
#include "window.h"

struct DebugData
{
	std::array<std::string, 0x10000> code;
	int known_bytes = 0;
	int all_bytes = 0x8000;
	bool mirror = false;

	void Clear()
	{
		known_bytes = 0;
		for (int i = 0; i < 0x10000; i++)
		{
			code[i].clear();
		}
	}
};

class Text;
class Button;
struct MachineStatus;
class AsmList;

class Debugger
{
private:
	
	static const int win_width = 1000;
	static const int win_height = 700;

	Text *text_status;
	Button* button_attach;
	AsmList* asm_list;
public:
	Window window;
	void Init();
	void DrawBackground(SDL_Renderer* renderer);
	void Button1Click();
	void Checkbox1Click(bool *new_state);
	void Open();
	void Close();
	void Update();
	void Attach();
	void Detach();

	DebugData debug_data;
	MachineStatus *machine_status;
	bool* debug_mode = nullptr;
	int mapper = -1;
};

