#include "asm_list.h"
#include "text.h"
#include "debugger.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"

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
	this->SetActive(false);
	this->renderer = renderer;
	SetRect(x, y, w, h * 14);
	elements = new Text * [h];
	this->cursor = cursor;
	this->num_elements = h;
	this->debug_data = debug_data;
	this->current_active_list = current_active_list;
	this->can_scroll_down = new bool[h];
	//Init();
	for (int i = 0; i < num_elements; i++)
	{
		can_scroll_down[h] = true;
		elements[i] = new Text(renderer, x, y + i * 14,"placeholder", 14);
	}
}

void AsmList::Render()
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, GetRect());

	if (IsActive())
	{
		for (int i = 0; i < num_elements; i++)
		{
			elements[i]->Render();
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

void AsmList::HandleEvent(SDL_Event* e)
{
	if (pressed && e->type == SDL_MOUSEMOTION)
	{
		SDL_Point point = { e->motion.x,e->motion.y };
		if (!SDL_PointInRect(&point, GetRect()))
			pressed = false;
	}
	else if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
	{
		SDL_Point point = { e->motion.x,e->motion.y };
		if (pressed && SDL_PointInRect(&point, GetRect()))
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
		if(SDL_PointInRect(&point, GetRect()))
			pressed = true;
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
	for (int i = 0; i < num_elements; i++)
	{
		for (int j = counter; j <= 0xffff; j++)
		{
			if (!debug_data->code[j].empty())
			{
				if (i > 0 && debug_data->code[j - 1].empty() && debug_data->code[j - 2].empty() && debug_data->code[j - 3].empty() && !prot)
				{
					elements[i]->SetText("...");
					counter = j;
					prot = true;
					can_scroll_down[i] = true;
					break;
				}
				elements[i]->SetText(debug_data->code[j]);
				counter = j + 1;
				prot = false;
				can_scroll_down[i] = true;
				break;
			}
			else if (j == 0xffff && debug_data->code[j].empty())
			{
				elements[i]->SetText("---------------------------------");
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
