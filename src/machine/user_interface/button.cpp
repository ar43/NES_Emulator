#include "button.h"
#include "../../logger/logger.h"
#include "text.h"

Button::Button(SDL_Renderer *renderer, int x, int y, int w, int h, std::string text, std::function<void()> OnClick)
{
	this->renderer = renderer;
	this->text = text;

	SetColor(base_color, base_color, base_color);
	text_obj = new Text(renderer, x, y, text,true,w,h);
	if (w == 0)
	{
		w = text_obj->SimulateWidth(text_obj->GetText())+10;
		delete text_obj;
		text_obj = new Text(renderer, x, y, text,true,w,h);
		//text_obj->SetText(text, true, w, h);
	}
	this->OnClick = OnClick;
	SetRect(x, y, w, h);
}

void Button::SetText(std::string text)
{
	//text_obj->SetText(text, true, GetRect()->w, GetRect()->h);
	delete text_obj;
	text_obj = new Text(this->renderer, GetRect()->x, GetRect()->y, text,true,GetRect()->w,GetRect()->h);
}

bool Button::HandleEvent(SDL_Event* e, Uint32* current_active_element)
{
	bool retvalue = false;
	SDL_Point point = { e->motion.x,e->motion.y };

	if (e->type == SDL_MOUSEMOTION)
	{
		if (SDL_PointInRect(&point, GetRect()))
		{
			hover = true;
		}
		else
		{
			hover = false;
		}
	}

	if (pressed && e->type == SDL_MOUSEMOTION)
	{
		if (!SDL_PointInRect(&point, GetRect()))
			pressed = false;
	}
	else if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
	{
		if (pressed && SDL_PointInRect(&point, GetRect()))
		{
			pressed = false;
			if (IsActive())
			{
				if(OnClick)
					OnClick();
				else
					logger::PrintLine(logger::LogType::DEBUG, "Button #" + std::to_string(GetId()) + " clicked but has no function bound");
			}
			//logger::PrintLine(logger::LogType::DEBUG, "Button #" + std::to_string(GetId()) + " clicked");
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
	{
		//logger::PrintLine(logger::LogType::DEBUG, "zzz");
		if (SDL_PointInRect(&point, GetRect()))
			pressed = true;
		else
			retvalue = false;
	}

	if (listen)
	{
		if (e->type == SDL_KEYDOWN)
		{
			key = e->key.keysym.sym;
			read = true;
			listen = false;
		}
	}
	return retvalue;
}

void Button::Render(Uint32* current_active_element)
{
	SDL_Color final_color = {GetColor()->r,GetColor()->g,GetColor()->b,GetColor()->a};
	SDL_Color outer_color = {0,0,0,GetColor()->a};
	SDL_Rect inner = { GetRect()->x + 1,GetRect()->y + 1,GetRect()->w - 2,GetRect()->h - 2 };
	if (pressed && IsActive())
	{
		final_color.r -= 0x10;
		final_color.g -= 0x10;
		final_color.b -= 0x10;
		outer_color.r = final_color.r;
		outer_color.g = final_color.g;
		outer_color.b = final_color.b;
	}
	else if (hover && IsActive())
	{
		outer_color.r = 52;
		outer_color.g = 119;
		outer_color.b = 235;
	}
	SDL_SetRenderDrawColor(renderer, outer_color.r, outer_color.g, outer_color.b, outer_color.a);
	if(!IsActive())
		SDL_SetRenderDrawColor(renderer, final_color.r, final_color.g, final_color.b, final_color.a);
	SDL_RenderFillRect(renderer, GetRect());
	SDL_SetRenderDrawColor(renderer, final_color.r, final_color.g, final_color.b, final_color.a);
	SDL_RenderFillRect(renderer, &inner);
	if (text_obj != nullptr)
		text_obj->Render(current_active_element);
}

int Button::GetWidth()
{
	return GetRect()->w;
}

void Button::SetActive(bool active)
{
	Element::SetActive(active);
	if (active == false)
	{
		SetColor(201, 201, 201);
		text_obj->SetColor(161, 161, 161);
		text_obj->SetText(text);
	}
	else
	{
		SetColor(base_color, base_color, base_color);
		text_obj->SetColor(0, 0, 0);
		text_obj->SetText(text);
	}
}