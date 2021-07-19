#include "element.h"

Uint32 Element::ID = 0;

SDL_Rect *Element::GetRect()
{
	return &rect;
}

Uint32 Element::GetId()
{
	return id;
}

void Element::SetPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Element::SetRect(int x, int y, int w, int h)
{
	SetPosition(x, y);
	rect.w = w;
	rect.h = h;
}

void Element::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 0xff;
}

SDL_Color* Element::GetColor()
{
	return &color;
}
