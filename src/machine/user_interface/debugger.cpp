#include "debugger.h"
#include "../../logger/logger.h"
#include "button.h"

void Debugger::Init(TTF_Font *font)
{
    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    window.OnOpen = std::bind(&Debugger::Open,this);
    window.OnClose = std::bind(&Debugger::Close,this);
    window.DrawHook = std::bind(&Debugger::DrawBackground, this, std::placeholders::_1);
    window.AddText(50, 10, "Debugger", font, 18);
    window.AddButton(10,50,73,21,"Okay", font, std::bind(&Debugger::Button1Click, this));
    window.AddCheckbox(100, 100, "Some stuff here", font, std::bind(&Debugger::Checkbox1Click, this, std::placeholders::_1));
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

void Debugger::Open()
{
    *debug_mode = true;
}

void Debugger::Close()
{
    *debug_mode = false;
}

