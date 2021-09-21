#include "text.h"
#include "../../logger/logger.h"
#include <SDL_ttf.h>

void Text::Render(Uint32* current_active_element)
{
    SDL_RenderCopy( renderer, texture, NULL, GetRect() );
}

void Text::SetText(std::string text, bool offset, int w, int h)
{
    this->text = text;
    if (text == "")
        text = " ";

    if (render_limit > 0)
    {
        int limiter = text.size();
        while (SimulateWidth(text) > render_limit)
        {
            limiter--;
            text = text.substr(text.size() - limiter, limiter);
        }
    }

    if (texture != NULL)
        SDL_DestroyTexture(texture);

    SDL_Surface* textSurface = TTF_RenderText_Blended(this->font, text.c_str(), {GetColor()->r,GetColor()->g,GetColor()->b,GetColor()->a});
    if( textSurface == NULL )
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to create text surface: " + std::string(TTF_GetError()));
    }
    else
    {
        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == NULL )
        {
            logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to create text texture: " + std::string(TTF_GetError()));
        }
        else
        {
            if(offset)
                SetRect(GetRect()->x+w/2-textSurface->w/2, GetRect()->y + ((h - size) / 2), textSurface->w, textSurface->h);
            else
                SetRect(GetRect()->x, GetRect()->y, textSurface->w, textSurface->h);
        }
        this->w = textSurface->w;
        this->h = textSurface->h;
        SDL_FreeSurface( textSurface );
    }
}

std::string Text::GetText()
{
    return text;
}

int Text::GetHeight()
{
    return h;
}

int Text::GetWidth()
{
    return w;
}

int Text::SimulateWidth(std::string in_text)
{
    if (GetText().size() > 0)
    {
        int w, h;
        TTF_SizeText(this->font, in_text.c_str(), &w, &h);
        return w;
    }
    else
        return 0;
}
