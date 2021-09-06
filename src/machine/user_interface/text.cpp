#include "text.h"
#include "../../logger/logger.h"
#include <SDL_ttf.h>

void Text::Render(SDL_Renderer* renderer)
{
    SDL_RenderCopy( renderer, texture, NULL, GetRect() );
}

void Text::SetText(SDL_Renderer *renderer, std::string text, bool offset, int w, int h)
{
    this->text = text;
    if (texture != NULL)
        SDL_DestroyTexture(texture);

    SDL_Surface* textSurface = TTF_RenderText_Blended(this->font, GetText().c_str(), {GetColor()->r,GetColor()->g,GetColor()->b,GetColor()->a});
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

        SDL_FreeSurface( textSurface );
    }
}

std::string Text::GetText()
{
    return text;
}
