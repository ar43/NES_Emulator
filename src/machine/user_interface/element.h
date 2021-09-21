#pragma once
#include <SDL.h>

class Element
{
private:

	
	Uint32 id;
	static Uint32 ID;

	SDL_Rect rect;
	SDL_Color color;

	bool active = true;

protected:
	SDL_Renderer* renderer;

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
	void SetActive(bool active);
	bool IsActive();
	SDL_Color* GetColor();
	virtual bool HandleEvent(SDL_Event* e, Uint32* current_active_element) = 0;
	virtual void Render(Uint32* current_active_element) = 0;
};