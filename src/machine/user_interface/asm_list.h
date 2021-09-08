#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
#include "element.h"

class Text;
struct DebugData;

class AsmList : public Element
{
private:
	
	int num_elements;
	DebugData* debug_data;
	Text** elements;
	bool* can_scroll_down;
	bool CanScrollDown();
	bool pressed;
public:
	int cursor;
	Uint32* current_active_list;
	AsmList(SDL_Renderer* renderer, int x, int y, int w, int h, int cursor, DebugData* debug_data, Uint32* current_active_list);
	void HandleEvent(SDL_Event* e);
	void Update();
	void InitCursor(int start_loc, bool down = false, bool force = false);
	void Render();
	void ScrollUp(int speed);
	void ScrollDown(int speed);
};