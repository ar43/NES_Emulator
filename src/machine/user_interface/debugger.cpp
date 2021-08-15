#include "debugger.h"
#include "../../logger/logger.h"
#include "button.h"

void Debugger::Init(TTF_Font *font)
{
    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    window.DrawHook = &Debugger::DrawBackground;
    window.AddText(50, 10, "Debugger", font, 18);
    window.AddButton(10,50,73,21,"Okay", font, &Debugger::Button1Click);
    window.AddCheckbox(100, 100, "Some stuff here", font, &Debugger::Checkbox1Click);
}

void Debugger::DrawBackground(SDL_Renderer* renderer)
{
    SDL_Rect rect = { win_width-50,0,50,50 };
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Debugger::Button1Click()
{
    logger::PrintLine(logger::LogType::INFO, "placeholder button click");
}

void Debugger::Checkbox1Click(bool* new_state)
{
    if (*new_state == true)
    {
        logger::PrintLine(logger::LogType::INFO, "Checked");
    }
    else
    {
        logger::PrintLine(logger::LogType::INFO, "Unchecked");
    }
}

