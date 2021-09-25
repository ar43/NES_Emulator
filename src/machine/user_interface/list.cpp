#include "list.h"
#include "text.h"
#include "../../logger/logger.h"
#include "../../utility/utility.h"

void List::RenderSlider(SDL_Rect *rect_slider)
{
	int all = end - start;
	int y = (int)(GetRect()->y+List::slider_w + ((cursor-start) / (float)(all))*(GetRect()->h-List::slider_w*2-List::slider_h));
	if (elements[this->num_elements-1].text->GetText().c_str()[0] == '-') //hack but its fast
		y = GetRect()->y + GetRect()->h - List::slider_w-List::slider_h;
	SDL_SetRenderDrawColor(renderer, 38, 38, 38, 0xff);
	rect_slider->y = y;
	SDL_RenderFillRect(renderer, rect_slider);
}

void List::Render(Uint32* current_active_element)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, GetRect());

	SDL_Rect rect_slider_body = { GetRect()->x + GetRect()->w,GetRect()->y,List::slider_w,GetRect()->h };
	SDL_Rect rect_up = { GetRect()->x + GetRect()->w,GetRect()->y,List::slider_w,List::slider_w };
	SDL_Rect rect_down = { GetRect()->x + GetRect()->w,GetRect()->y+GetRect()->h-List::slider_w,List::slider_w,List::slider_w };
	SDL_Rect rect_slider = { GetRect()->x + GetRect()->w,GetRect()->y+List::slider_w,List::slider_w,List::slider_h };
	SDL_Rect rect_selected = { GetRect()->x, GetRect()->y + 0 * List::font_size,GetRect()->w,List::font_size };
	SDL_Rect rect_breakpoint_outer = { GetRect()->x + 2,GetRect()->y + 0 * List::font_size + 3,8,8 };
	SDL_Rect rect_breakpoint_inner = { GetRect()->x + 3,GetRect()->y + 0 * List::font_size + 4,6,6 };

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

			if (elements[i].number == selected)
			{
				rect_selected.y = GetRect()->y + i * List::font_size;
				SDL_SetRenderDrawColor(renderer, 217, 255, 255, 0xff);
				SDL_RenderFillRect(renderer, &rect_selected);
			}

			if (elements[i].number == selected)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
				SDL_RenderDrawLine(renderer, GetRect()->x , rect_selected.y, GetRect()->x+GetRect()->w-1, rect_selected.y);
				SDL_RenderDrawLine(renderer, GetRect()->x , rect_selected.y+List::font_size, GetRect()->x+GetRect()->w-1, rect_selected.y+List::font_size);
				SDL_RenderDrawLine(renderer, GetRect()->x , rect_selected.y, GetRect()->x, rect_selected.y+List::font_size);
				SDL_RenderDrawLine(renderer, GetRect()->x+GetRect()->w-1 , rect_selected.y, GetRect()->x+GetRect()->w-1, rect_selected.y+List::font_size);

				//selected_string = " Selected " + utility::int_to_hex(selected) + ".";
			}
			elements[i].text->Render(current_active_element);
		}
		RenderSlider(&rect_slider);
	}
}

List::List(SDL_Renderer* renderer, int x, int y, int w, int h, std::vector<std::string>* data)
{
	this->renderer = renderer;
	SetRect(x, y, w, h * 14);
	elements = new ListElement[h];
	SetActive(false);
	this->data = data;
	this->num_elements = h;

	for (int i = 0; i < num_elements; i++)
	{
		elements[i].text = new Text(renderer, x , y + i * List::font_size,"placeholder", List::font_size);
	}
}



void List::ScrollUp(int speed)
{
	cursor -= speed;
	if (cursor < 0)
		cursor = 0;
	Update();
	logger::PrintLine(logger::LogType::DEBUG, "Cursor: " + utility::int_to_hex(this->cursor));
}

void List::ScrollDown(int speed)
{
	cursor += speed;
	if (cursor + this->num_elements - 1 >= (int)data->size())
		cursor = data->size() - this->num_elements;
	if (cursor < 0)
		cursor = 0;
	Update();
	logger::PrintLine(logger::LogType::DEBUG, "Cursor: " + utility::int_to_hex(this->cursor));
}

int List::GetSelected()
{
	return selected;
}

void List::SetSelected(int selected)
{
	this->selected = selected;
}

std::string List::GetSelectedText()
{
	if (selected >= 0 && selected < (int)data->size())
	{
		std::string txt = data->at(selected);
		return txt;
	}
	else
	{
		return "";
	}
}

bool List::HandleEvent(SDL_Event* e, Uint32* current_active_element)
{
	SDL_Rect rect_up = { GetRect()->x + GetRect()->w,GetRect()->y,List::slider_w,List::slider_w };
	SDL_Rect rect_down = { GetRect()->x + GetRect()->w,GetRect()->y+GetRect()->h-List::slider_w,List::slider_w,List::slider_w };
	SDL_Rect rect_slider_body = { GetRect()->x + GetRect()->w,GetRect()->y,List::slider_w,GetRect()->h };
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

				float c = (point.y - GetRect()->y - List::slider_w) / float(rect_slider_body.h - List::slider_w*2);
				int num = int(c * (end - start) + start);
				this->cursor = num;
				if (this->cursor >= (int)data->size())
					this->cursor = data->size() - 1;
				logger::PrintLine(logger::LogType::DEBUG, utility::int_to_hex(num));
				Update();
			}
			else if (SDL_PointInRect(&point,GetRect()))
			{
				int y = point.y - GetRect()->y;
				int element = y / List::font_size;
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

void List::Update()
{
	if (data->size() == 0)
	{
		SetActive(false);
		return;
	}
	if (cursor == -1)
	{
		InitCursor(0);
	}
	int counter = cursor;
	bool prot = false;
	//FindStartAndEnd();
	start = 0;
	end = data->size() - this->num_elements;
	if (end < 0)
		end = 0;
	for (int i = 0; i < num_elements; i++)
	{
		size_t j = counter;
		if (j < data->size())
		{
			elements[i].text->SetText(data->at(j));
			elements[i].number = j;
			counter = j + 1;
		}
		else
		{
			elements[i].text->SetText("");
			elements[i].number = -1;
		}
	}

}

void List::InitCursor(int start_loc, bool down, bool force)
{
	//if (start_loc < 0x8000)
	//	start_loc = 0x8000;

	//if (!down)
	//{
	//	for (int j = start_loc; j < 0xffff; j++)
	//	{
	//		if (!debug_data->code[j].empty())
	//		{
	//			this->cursor = j;
	//			return;
	//		}
	//	}
	//	if (force)
	//	{
	//		InitCursor(0xffff, true);
	//	}
	//	//this->cursor = 0xffff;
	//}
	//else
	//{
	//	for (int j = start_loc; j >= 0; j--)
	//	{
	//		if (!debug_data->code[j].empty())
	//		{
	//			this->cursor = j;
	//			return;
	//		}
	//	}
	//	//this->cursor = 0;
	//}
	this->cursor = 0;
}
