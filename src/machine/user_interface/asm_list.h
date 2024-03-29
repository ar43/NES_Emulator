#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
#include "element.h"

class Text;
struct DebugData;

struct AsmListElement
{
	Text *text;
	int number;
};

class AsmList : public Element
{
private:
	
	int num_elements;
	DebugData* debug_data;
	AsmListElement* elements;
	bool* can_scroll_down;
	bool CanScrollDown();
	bool pressed;
	int start, end;
	int selected = 0;

	static const int font_size = 14;
	static const int slider_w = 20;
	static const int slider_h = 10;

public:
	int cursor;

	AsmList(SDL_Renderer* renderer, int x, int y, int w, int h, int cursor, DebugData* debug_data);
	void RenderSlider(SDL_Rect *rect_slider);
	bool HandleEvent(SDL_Event* e, Uint32 *current_active_element);
	void Update();
	void InitCursor(int start_loc, bool down = false, bool force = false);
	void Render(Uint32* current_active_element);
	void ScrollUp(int speed);
	void ScrollDown(int speed);
	void FindStartAndEnd();
	int GetSelected();
	void SetSelected(int selected);

	/*std::string status_string;
	std::string selected_string;*/
};