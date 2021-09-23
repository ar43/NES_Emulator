#include "button.h"
#include "../../logger/logger.h"
#include "text.h"

Button::Button(SDL_Renderer *renderer, int x, int y, int w, int h, std::string text, std::function<void()> OnClick)
{
	this->renderer = renderer;
	this->text = text;
	
	SetColor(0xb1, 0xb1, 0xb1);
	text_obj = new Text(renderer, x, y, text,true,w,h);
	if (w == 0)
	{
		w = text_obj->SimulateWidth(text_obj->GetText())+10;
		delete text_obj;
		text_obj = new Text(renderer, x, y, text,true,w,h);
	}
	this->OnClick = OnClick;
	SetRect(x, y, w, h);
}

bool Button::HandleEvent(SDL_Event* e, Uint32* current_active_element)
{
	bool retvalue = false;
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
		SDL_Point point = { e->motion.x,e->motion.y };
		if (SDL_PointInRect(&point, GetRect()))
			pressed = true;
		else
			retvalue = false;
	}
	return retvalue;
}

void Button::Render(Uint32* current_active_element)
{
	SDL_Color final_color = {GetColor()->r,GetColor()->g,GetColor()->b,GetColor()->a};
	if (pressed && IsActive())
	{
		final_color.r -= 0x10;
		final_color.g -= 0x10;
		final_color.b -= 0x10;
	}
	SDL_SetRenderDrawColor(renderer, final_color.r, final_color.g, final_color.b, final_color.a);
	SDL_RenderFillRect(renderer, GetRect());
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
		SetColor(0xb1, 0xb1, 0xb1);
		text_obj->SetColor(0, 0, 0);
		text_obj->SetText(text);
	}
}