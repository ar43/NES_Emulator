#include "debugger.h"
#include "../../logger/logger.h"
#include "text.h"
#include "button.h"
#include "../misc/machine_status.h"
#include <string>
#include <iomanip>
#include <sstream>

void Debugger::Init()
{
    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    window.OnOpen = std::bind(&Debugger::Open,this);
    window.OnClose = std::bind(&Debugger::Close,this);
    window.OnUpdate = std::bind(&Debugger::Update, this);
    window.DrawHook = std::bind(&Debugger::DrawBackground, this, std::placeholders::_1);
    text_status = window.AddText(3, win_height-12, "Status: Game is not running", 12);
    button_attach = window.AddButton(10,50,73,21,"Attach", std::bind(&Debugger::Attach, this));
    window.AddCheckbox(100, 100, "Some stuff here", std::bind(&Debugger::Checkbox1Click, this, std::placeholders::_1));
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
    text_status->SetText(window.GetRenderer(), "Changed");
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

void Debugger::Attach()
{
    if (machine_status->running == RunningStatus::RUNNING)
    {
        *debug_mode = false;
        text_status->SetText(window.GetRenderer(), "Status: Game is not running");
        button_attach->SetActive(true);
    }
    else if (!(mapper == 0 || mapper == 3))
    {
        *debug_mode = false;
        text_status->SetText(window.GetRenderer(), "Status: Unsupported mapper");
        button_attach->SetActive(true);
    }
    else if(machine_status->running == RunningStatus::RUNNING_ROM)
    {
        *debug_mode = true;
        text_status->SetText(window.GetRenderer(), "Status: Attached");
        button_attach->SetActive(false);
    }
        

}

void Debugger::Detach()
{
    if (*debug_mode == true)
    {
        *debug_mode = false;
        text_status->SetText(window.GetRenderer(), "Status: Detached");
        button_attach->SetActive(true);
        debug_data.Clear();
    }
}

void Debugger::Open()
{
    Attach();
}

void Debugger::Close()
{
    Detach();
}

void Debugger::Update()
{
    static int last_update = SDL_GetTicks();
    static RunningStatus last_running_status = RunningStatus::NOT_RUNNING;
    if (*debug_mode == true)
    {
        if (SDL_GetTicks() - last_update > 3000)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << double(debug_data.known_bytes) / double(debug_data.all_bytes) * 100;
            std::string str = "Status: Attached - " + stream.str() + "% of PRG code discovered";
            text_status->SetText(window.GetRenderer(), str);
            last_update = SDL_GetTicks();
        }
    }
    if (last_running_status == RunningStatus::RUNNING_ROM && machine_status->running == RunningStatus::RUNNING)
    {
        Detach();
    }
    last_running_status = machine_status->running;
}

