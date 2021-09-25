#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>
#include <vector>
#include "element.h"

class Text;

struct ListElement
{
	Text *text;
	int number;
};

class List : public Element
{
private:
	std::vector<std::string> *data;

	int num_elements;
	ListElement* elements;
	bool* can_scroll_down;
	bool CanScrollDown();
	bool pressed;
	int start, end;
	int selected = 0;

	static const int font_size = 14;
	static const int slider_w = 10;
	static const int slider_h = 5;
public:
	int cursor;
	List(SDL_Renderer* renderer, int x, int y, int w, int h, std::vector<std::string>* data);

	void RenderSlider(SDL_Rect *rect_slider);
	bool HandleEvent(SDL_Event* e, Uint32 *current_active_element);
	void Update();
	void InitCursor(int start_loc, bool down = false, bool force = false);
	void Render(Uint32* current_active_element);
	void ScrollUp(int speed);
	void ScrollDown(int speed);
	int GetSelected();
	void SetSelected(int selected);
};