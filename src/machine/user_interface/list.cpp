#include "list.h"

void List::Render()
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, GetRect());
}

void List::HandleEvent(SDL_Event* e)
{
	
}
