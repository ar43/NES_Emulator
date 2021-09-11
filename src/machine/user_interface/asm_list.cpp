#include "asm_list.h"
#include "text.h"
#include "debugger.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"
#include <string>
#include <sstream>
#include <iostream>

bool AsmList::CanScrollDown()
{
	for (int i = 0; i < num_elements; i++)
	{
		if (can_scroll_down[i] == false)
			return false;
	}
	return true;
}

AsmList::AsmList(SDL_Renderer* renderer, int x, int y, int w, int h, int cursor, DebugData* debug_data, Uint32* current_active_list)
{
	static int asm_offset = 12;
	this->SetActive(false);
	this->renderer = renderer;
	SetRect(x, y, w, h * AsmList::font_size);
	elements = new AsmListElement[h];
	this->cursor = cursor;
	this->num_elements = h;
	this->debug_data = debug_data;
	this->current_active_list = current_active_list;
	this->can_scroll_down = new bool[h];
	//Init();
	for (int i = 0; i < num_elements; i++)
	{
		can_scroll_down[h] = true;
		elements[i].text = new Text(renderer, x+asm_offset , y + i * AsmList::font_size,"placeholder", AsmList::font_size);
	}
}

void AsmList::RenderSlider(SDL_Rect *rect_slider)
{
	int all = end - start;
	int y = (int)(GetRect()->y+AsmList::slider_w + ((cursor-start) / (float)(all))*(GetRect()->h-AsmList::slider_w*2-AsmList::slider_h));
	if (elements[this->num_elements-1].text->GetText().c_str()[0] == '-') //hack but its fast
		y = GetRect()->y + GetRect()->h - AsmList::slider_w-AsmList::slider_h;
	SDL_SetRenderDrawColor(renderer, 38, 38, 38, 0xff);
	rect_slider->y = y;
	SDL_RenderFillRect(renderer, rect_slider);
}

void AsmList::Render()
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, GetRect());

	static SDL_Rect rect_slider_body = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,GetRect()->h };
	static SDL_Rect rect_up = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_down = { GetRect()->x + GetRect()->w,GetRect()->y+GetRect()->h-AsmList::slider_w,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_slider = { GetRect()->x + GetRect()->w,GetRect()->y+AsmList::slider_w,AsmList::slider_w,AsmList::slider_h };
	static SDL_Rect rect_selected = { GetRect()->x, GetRect()->y + 0 * AsmList::font_size,GetRect()->w,AsmList::font_size };

	SDL_SetRenderDrawColor(renderer, 230, 230, 230, 0xff);
	SDL_RenderFillRect(renderer, &rect_slider_body);
	SDL_SetRenderDrawColor(renderer, 64, 64, 64, 0xff);
	SDL_RenderFillRect(renderer, &rect_up);
	SDL_RenderFillRect(renderer, &rect_down);

	if (IsActive())
	{
		for (int i = 0; i < num_elements; i++)
		{
			if (elements[i].number == selected)
			{
				rect_selected.y = GetRect()->y + i * AsmList::font_size;
				SDL_SetRenderDrawColor(renderer, 102, 226, 242, 0xff);
				SDL_RenderFillRect(renderer, &rect_selected);
			}
			elements[i].text->Render();
		}
		RenderSlider(&rect_slider);
	}
}

void AsmList::ScrollUp(int speed)
{
	InitCursor(cursor - speed,true);
	Update();
	logger::PrintLine(logger::LogType::DEBUG, "Cursor: " + utility::int_to_hex(this->cursor));
}

void AsmList::ScrollDown(int speed)
{
	if (!CanScrollDown())
		return;

	InitCursor(cursor + speed);
	Update();
	logger::PrintLine(logger::LogType::DEBUG, "Cursor: " + utility::int_to_hex(this->cursor));
}

void AsmList::FindStartAndEnd()
{
	for (int i = 0x8000; i < 0xffff; i++)
	{
		if (!debug_data->code[i].empty())
		{
			start = i;
			break;
		}
	}
	for (int i = 0xffff; i >= 0x8000; i--)
	{
		if (!debug_data->code[i].empty())
		{
			end = i;
			break;
		}
	}
}

void AsmList::HandleEvent(SDL_Event* e)
{
	static SDL_Rect rect_up = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_down = { GetRect()->x + GetRect()->w,GetRect()->y+GetRect()->h-AsmList::slider_w,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_slider_body = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,GetRect()->h };

	if (pressed && e->type == SDL_MOUSEMOTION)
	{
		SDL_Point point = { e->motion.x,e->motion.y };
		if (!(SDL_PointInRect(&point, GetRect()) || SDL_PointInRect(&point,&rect_slider_body)))
			pressed = false;
	}
	else if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
	{
		SDL_Point point = { e->motion.x,e->motion.y };
		if (pressed && (SDL_PointInRect(&point, GetRect()) || SDL_PointInRect(&point,&rect_slider_body)))
		{
			pressed = false;
			if (IsActive())
			{
				*current_active_list = GetId();
			}
			//logger::PrintLine(logger::LogType::DEBUG, "Button #" + std::to_string(GetId()) + " clicked");
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
	{
		//logger::PrintLine(logger::LogType::DEBUG, "zzz");
		SDL_Point point = { e->motion.x,e->motion.y };
		if((SDL_PointInRect(&point, GetRect()) || SDL_PointInRect(&point,&rect_slider_body)))
			pressed = true;

		if (IsActive())
		{
			if (SDL_PointInRect(&point, &rect_up))
				ScrollUp(5);
			else if (SDL_PointInRect(&point, &rect_down))
				ScrollDown(5);
			else if (SDL_PointInRect(&point, &rect_slider_body))
			{
				
				float c = (point.y - GetRect()->y - AsmList::slider_w) / float(rect_slider_body.h - AsmList::slider_w*2);
				int num = int(c * (end - start) + start);
				InitCursor(num, false, true);
				logger::PrintLine(logger::LogType::DEBUG, utility::int_to_hex(num));
				Update();
			}
			else if (SDL_PointInRect(&point,GetRect()))
			{
				int y = point.y - GetRect()->y;
				int element = y / AsmList::font_size;
				selected = elements[element].number;
				if (selected < 0)
					selected = 0;
				logger::PrintLine(logger::LogType::DEBUG, "Selected: " + utility::int_to_hex(selected));
			}
				
		}
	}
	else if(e->type == SDL_MOUSEWHEEL && *current_active_list == GetId() && IsActive())
	{
		if(e->wheel.y > 0) // scroll up
		{
			ScrollUp(1);
		}
		else if(e->wheel.y < 0) // scroll down
		{
			ScrollDown(1);
		}
	}
	else if (e->type == SDL_KEYDOWN && *current_active_list == GetId() && IsActive())
	{
		switch (e->key.keysym.sym)
		{
			case SDLK_PAGEUP:
			{
				ScrollUp(this->num_elements);
				break;
			}
			case SDLK_PAGEDOWN:
			{
				ScrollDown(this->num_elements);
				break;
			}
			default: break;
			
		}
	}
}

void AsmList::Update()
{
	if (cursor == -1)
	{
		InitCursor(0x8000);
	}
	int counter = cursor;
	bool prot = false;
	FindStartAndEnd();
	for (int i = 0; i < num_elements; i++)
	{
		for (int j = counter; j <= 0xffff; j++)
		{
			if (!debug_data->code[j].empty())
			{
				if (i > 0 && debug_data->code[j - 1].empty() && debug_data->code[j - 2].empty() && debug_data->code[j - 3].empty() && !prot)
				{
					elements[i].text->SetText("...");
					elements[i].number = -1;
					counter = j;
					prot = true;
					can_scroll_down[i] = true;
					break;
				}
				elements[i].text->SetText(debug_data->code[j]);
				std::string hex_num = debug_data->code[j].substr(0, 4);
				std::stringstream ss;
				ss << std::hex << hex_num;
				ss >> elements[i].number;
				counter = j + 1;
				prot = false;
				can_scroll_down[i] = true;
				break;
			}
			else if (j == 0xffff && debug_data->code[j].empty())
			{
				elements[i].text->SetText("---------------------------------");
				elements[i].number = -1;
				can_scroll_down[i] = false;
				break;
			}
		}
	}
	
}

void AsmList::InitCursor(int start_loc, bool down, bool force)
{
	if (start_loc < 0x8000)
		start_loc = 0x8000;

	if (!down)
	{
		for (int j = start_loc; j < 0xffff; j++)
		{
			if (!debug_data->code[j].empty())
			{
				this->cursor = j;
				return;
			}
		}
		if (force)
		{
			InitCursor(0xffff, true);
		}
		//this->cursor = 0xffff;
	}
	else
	{
		for (int j = start_loc; j >= 0; j--)
		{
			if (!debug_data->code[j].empty())
			{
				this->cursor = j;
				return;
			}
		}
		//this->cursor = 0;
	}
	
}
