#include "button.h"
#include "../../logger/logger.h"

void Button::HandleEvent(SDL_Event* e)
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
			//onClick();
			logger::PrintLine(logger::LogType::DEBUG, "Button #" + std::to_string(GetId()) + " clicked");
		}
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
	{
		logger::PrintLine(logger::LogType::DEBUG, "zzz");
		SDL_Point point = { e->motion.x,e->motion.y };
		if(SDL_PointInRect(&point, GetRect()))
			pressed = true;
	}
}

void Button::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, GetColor()->r, GetColor()->g, GetColor()->b, GetColor()->a);
	SDL_RenderFillRect(renderer, GetRect());
}