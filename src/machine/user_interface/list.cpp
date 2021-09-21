#include "list.h"

void List::Render(Uint32* current_active_element)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, GetRect());
}

bool List::HandleEvent(SDL_Event* e, Uint32* current_active_element)
{
	return false;
}
