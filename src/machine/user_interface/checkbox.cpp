#include <SDL_image.h>
#include "checkbox.h"
#include "../../logger/logger.h"
#include "text.h"

Checkbox::Checkbox(SDL_Renderer *renderer, int x, int y, std::string text, bool start_state, std::function<void(bool*)> OnClick)
{
	this->renderer = renderer;
	InitTexture();
	this->text = text;
	SetRect(x, y, w, h);
	SetColor(0xff, 0xff, 0xff);
	text_obj = new Text(renderer, x+19, y, text, 16);
	this->OnClick = OnClick;
	this->state = start_state;
}

void Checkbox::InitTexture()
{
	SDL_Surface* loadedSurface = IMG_Load( CHECKMARK_PATH );
	if( loadedSurface == NULL )
	{
		logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to load image " + std::string(CHECKMARK_PATH) + " SDL_image Error: " + std::string(IMG_GetError()) );
	}
	else
	{
		texture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( texture == NULL )
		{
			logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to create checkmark texture! SDL_image Error: " + std::string(SDL_GetError()) );
		}

		SDL_FreeSurface( loadedSurface );
	}
}

bool Checkbox::HandleEvent(SDL_Event* e, Uint32* current_active_element)
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
				state = !state;
				if(OnClick)
					OnClick(&state);
				else
					logger::PrintLine(logger::LogType::DEBUG, "Checkbox #" + std::to_string(GetId()) + " clicked but has no function bound");
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

void Checkbox::Render(Uint32* current_active_element)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_Rect rect = { GetRect()->x - 1,GetRect()->y - 1, 16, 16 };
	SDL_RenderFillRect(renderer, &rect);
	SDL_Color final_color = {GetColor()->r,GetColor()->g,GetColor()->b,GetColor()->a};
	if (pressed && IsActive())
	{
		final_color.r -= 0x10;
		final_color.g -= 0x10;
		final_color.b -= 0x10;
	}
	SDL_SetRenderDrawColor(renderer, final_color.r, final_color.g, final_color.b, final_color.a);
	SDL_RenderFillRect(renderer, GetRect());
	if (state)
	{
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}
	if (text_obj != nullptr)
		text_obj->Render(current_active_element);
}