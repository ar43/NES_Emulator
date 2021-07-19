#pragma once
#include <SDL.h>

class Element
{
private:

	
	Uint32 id;
	static Uint32 ID;

	SDL_Rect rect;
	SDL_Color color;

public:
	Element()
	{
		id = ID++;
	}
	SDL_Rect *GetRect();
	Uint32 GetId();
	void SetPosition(int x, int y);
	void SetRect(int x, int y, int w, int h);
	void SetColor(Uint8 r, Uint8 g, Uint8 b);
	SDL_Color* GetColor();
	virtual void HandleEvent(SDL_Event* e) = 0;
	virtual void Render(SDL_Renderer *renderer) = 0;
};