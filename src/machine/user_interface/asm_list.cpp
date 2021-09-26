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

AsmList::AsmList(SDL_Renderer* renderer, int x, int y, int w, int h, int cursor, DebugData* debug_data)
{
	static int asm_offset = 12;
	this->SetActive(false);
	this->renderer = renderer;
	SetRect(x, y, w, h * AsmList::font_size);
	elements = new AsmListElement[h];
	this->cursor = cursor;
	this->num_elements = h;
	this->debug_data = debug_data;
	this->can_scroll_down = new bool[h];
	/*status_string = "";
	selected_string = "";*/
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

void AsmList::Render(Uint32* current_active_element)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, GetRect());

	static SDL_Rect rect_slider_body = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,GetRect()->h };
	static SDL_Rect rect_up = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_down = { GetRect()->x + GetRect()->w,GetRect()->y+GetRect()->h-AsmList::slider_w,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_slider = { GetRect()->x + GetRect()->w,GetRect()->y+AsmList::slider_w,AsmList::slider_w,AsmList::slider_h };
	static SDL_Rect rect_selected = { GetRect()->x, GetRect()->y + 0 * AsmList::font_size,GetRect()->w,AsmList::font_size };
	static SDL_Rect rect_breakpoint_outer = { GetRect()->x + 2,GetRect()->y + 0 * AsmList::font_size + 3,8,8 };
	static SDL_Rect rect_breakpoint_inner = { GetRect()->x + 3,GetRect()->y + 0 * AsmList::font_size + 4,6,6 };

	SDL_SetRenderDrawColor(renderer, 230, 230, 230, 0xff);
	SDL_RenderFillRect(renderer, &rect_slider_body);
	SDL_SetRenderDrawColor(renderer, 64, 64, 64, 0xff);
	SDL_RenderFillRect(renderer, &rect_up);
	SDL_RenderFillRect(renderer, &rect_down);

	bool force_cursor = true;

	if (IsActive())
	{
		for (int i = 0; i < num_elements; i++)
		{
			
			if (elements[i].number == debug_data->breakpoint_hit)
			{
				rect_selected.y = GetRect()->y + i * AsmList::font_size;
				SDL_SetRenderDrawColor(renderer, 255, 67, 54, 0xff);
				SDL_RenderFillRect(renderer, &rect_selected);
				force_cursor = false;
			}
			else if (elements[i].number == debug_data->hit)
			{
				rect_selected.y = GetRect()->y + i * AsmList::font_size;
				SDL_SetRenderDrawColor(renderer, 117, 255, 122, 0xff);
				SDL_RenderFillRect(renderer, &rect_selected);
				force_cursor = false;
			}
			else if (elements[i].number == selected)
			{
				rect_selected.y = GetRect()->y + i * AsmList::font_size;
				SDL_SetRenderDrawColor(renderer, 217, 255, 255, 0xff);
				SDL_RenderFillRect(renderer, &rect_selected);
			}

			if (elements[i].number == selected)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
				SDL_RenderDrawLine(renderer, GetRect()->x , rect_selected.y, GetRect()->x+GetRect()->w-1, rect_selected.y);
				SDL_RenderDrawLine(renderer, GetRect()->x , rect_selected.y+AsmList::font_size, GetRect()->x+GetRect()->w-1, rect_selected.y+AsmList::font_size);
				SDL_RenderDrawLine(renderer, GetRect()->x , rect_selected.y, GetRect()->x, rect_selected.y+AsmList::font_size);
				SDL_RenderDrawLine(renderer, GetRect()->x+GetRect()->w-1 , rect_selected.y, GetRect()->x+GetRect()->w-1, rect_selected.y+AsmList::font_size);

				//selected_string = " Selected " + utility::int_to_hex(selected) + ".";
			}

			if (elements[i].number >= 0x8000 && debug_data->breakpoint[elements[i].number] != Breakpoint::INACTIVE)
			{
				rect_breakpoint_outer.y = GetRect()->y + i * AsmList::font_size + 3;
				rect_breakpoint_inner.y = GetRect()->y + i * AsmList::font_size + 4;
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
				SDL_RenderFillRect(renderer, &rect_breakpoint_outer);
				if(debug_data->breakpoint[elements[i].number] == Breakpoint::ACTIVE)
					SDL_SetRenderDrawColor(renderer, 148, 17, 7, 0xff);
				else
					SDL_SetRenderDrawColor(renderer, 94, 86, 86, 0xff);
				SDL_RenderFillRect(renderer, &rect_breakpoint_inner);
			}
			elements[i].text->Render(current_active_element);
		}
		RenderSlider(&rect_slider);

		if (debug_data->signal == DebuggerSignal::PAUSE)
		{
			if (force_cursor && debug_data->force_cursor != 0)
			{
				InitCursor(debug_data->force_cursor, false, true);
				selected = debug_data->force_cursor;
				debug_data->force_cursor = 0;
				Update();
			}
			else if(debug_data->force_cursor != 0)
			{
				selected = debug_data->force_cursor;
				debug_data->force_cursor = 0;
			}
		}

		
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

int AsmList::GetSelected()
{
	return selected;
}

void AsmList::SetSelected(int selected)
{
	this->selected = selected;
}

bool AsmList::HandleEvent(SDL_Event* e, Uint32* current_active_element)
{
	static SDL_Rect rect_up = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_down = { GetRect()->x + GetRect()->w,GetRect()->y+GetRect()->h-AsmList::slider_w,AsmList::slider_w,AsmList::slider_w };
	static SDL_Rect rect_slider_body = { GetRect()->x + GetRect()->w,GetRect()->y,AsmList::slider_w,GetRect()->h };
	bool retvalue = true;

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
				*current_active_element = GetId();
			}
			//logger::PrintLine(logger::LogType::DEBUG, "Button #" + std::to_string(GetId()) + " clicked");
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
	{
		//logger::PrintLine(logger::LogType::DEBUG, "zzz");
		SDL_Point point = { e->motion.x,e->motion.y };
		if ((SDL_PointInRect(&point, GetRect()) || SDL_PointInRect(&point, &rect_slider_body)))
			pressed = true;
		else
			retvalue = false;

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
	else if(e->type == SDL_MOUSEWHEEL && *current_active_element == GetId() && IsActive())
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
	else if (e->type == SDL_KEYDOWN && *current_active_element == GetId() && IsActive())
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
	return retvalue;
}

void AsmList::Update()
{
	if (cursor == -1)
	{
		InitCursor(0x8000);
	}
	int counter = cursor;
	bool prot = false;
	bool prot2 = false;
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
				if (debug_data->is_subroutine[j] && !prot2)
				{
					prot2 = true;
					elements[i].text->SetText("SUBROUTINE " + utility::int_to_hex(j));
					prot = true;
					elements[i].number = -1;
					counter = j;
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
				prot2 = false;
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
