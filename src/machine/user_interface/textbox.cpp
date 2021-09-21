#include "textbox.h"
#include "text.h"

Textbox::Textbox(SDL_Renderer* renderer, int x, int y, int w, std::string text)
{
	this->renderer = renderer;
	SetActive(true);
	SetColor(0xff, 0xff, 0xff);
	SetRect(x, y, w, 18);
	this->text = text;
	text_obj = new Text(renderer, x+2, y+2, text,Textbox::text_size);
	text_obj->render_limit = w;
}

bool Textbox::HandleEvent(SDL_Event* e, Uint32* current_active_element)
{
	bool retvalue = true;
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
				*current_active_element = GetId();
				if(!SDL_IsTextInputActive())
					SDL_StartTextInput();
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
	else if( e->type == SDL_KEYDOWN && *current_active_element == GetId() )
	{
		if( e->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0 && SDL_GetModState() & KMOD_SHIFT)
		{
			text = "";
			update_text = true;
		}
		else if( e->key.keysym.sym == SDLK_BACKSPACE && text.length() > 0 )
		{
			text.pop_back();
			update_text = true;
		}
		else if( e->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
		{
			SDL_SetClipboardText( text.c_str() );
		}
		else if( e->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
		{
			char* clipboard = SDL_GetClipboardText();
			if (clipboard != NULL && SDL_strlen(clipboard) < Textbox::MAX_INPUT)
				text = clipboard;
			if(clipboard != NULL)
				SDL_free(clipboard);
			update_text = true;
		}
	}
	else if (e->type == SDL_TEXTINPUT && *current_active_element == GetId())
	{
		if(text.size() < Textbox::MAX_INPUT)
			text += e->text.text;
		update_text = true;
	}
	return retvalue;
}

void Textbox::Render(Uint32* current_active_element)
{
	SDL_Rect inner = { GetRect()->x + 1,GetRect()->y + 1,GetRect()->w - 2,GetRect()->h - 2 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
	SDL_RenderFillRect(renderer, GetRect());
	SDL_SetRenderDrawColor(renderer, GetColor()->r, GetColor()->g, GetColor()->b, 0xff);
	SDL_RenderFillRect(renderer, &inner);
	if (text_obj != nullptr)
	{
		if (update_text)
		{
			update_text = false;
			text_obj->SetText(text);
		}
		text_obj->Render(current_active_element);
	}
	if (*current_active_element == GetId() && (SDL_GetTicks() / 500) & 1)
	{
		int w = text_obj->GetWidth();
		if (text == "")
			w = 0;
		SDL_Rect cursor = { GetRect()->x + 2 + w,GetRect()->y + 2,1,Textbox::text_size };
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &cursor);
	}
}

std::string Textbox::GetText()
{
	return text;
}
