#include "debugger.h"
#include "../../logger/logger.h"
#include "text.h"
#include "button.h"
#include "../misc/machine_status.h"
#include <string>
#include <iomanip>
#include <sstream>
#include "asm_list.h"

void Debugger::Init(SDL_Window* window_main)
{
    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    window.OnOpen = std::bind(&Debugger::Open,this);
    window.OnClose = std::bind(&Debugger::Close,this);
    window.OnUpdate = std::bind(&Debugger::Update, this);
    window.DrawHook = std::bind(&Debugger::DrawBackground, this, std::placeholders::_1);
    text_status = window.AddText(3, win_height-12, "Status: Game is not running", 12);
    button_attach = window.AddButton(10,50,73,21,"Attach", std::bind(&Debugger::Attach, this));
    button_breakpoint_toggle = window.AddButton(460,50,173,21,"Toggle breakpoint", std::bind(&Debugger::ToggleBreakpoint, this));
    button_continue = window.AddButton(460,100,173,21, "Continue", std::bind(&Debugger::Continue, this));
    button_step = window.AddButton(460,150,173,21, "Step In", std::bind(&Debugger::StepIn, this));
    button_step_over = window.AddButton(460,200,173,21, "Step Over", std::bind(&Debugger::StepOver, this));
    window.AddCheckbox(100, 51, "Some stuff here", std::bind(&Debugger::Checkbox1Click, this, std::placeholders::_1));
    asm_list = window.AddAsmList(10, 100, 300, 22,-1,&debug_data);
    this->window_main = window_main;
}

void Debugger::DrawBackground(SDL_Renderer* renderer)
{
    /*SDL_Rect rect = { win_width-50,0,50,50 };
    SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);*/
}

void Debugger::Continue()
{
    debug_data.signal = DebuggerSignal::CONTINUE;
    machine_status->paused = false;
}

void Debugger::StepIn()
{
    if (debug_data.signal == DebuggerSignal::PAUSE)
    {
        debug_data.step = 1;
    }
}

void Debugger::StepOver()
{
    if (debug_data.signal == DebuggerSignal::PAUSE)
    {
        debug_data.step = 2;
    }
}

void Debugger::Button1Click()
{
    logger::PrintLine(logger::LogType::INFO, "placeholder button click");
    text_status->SetText("Changed");
}

void Debugger::ToggleBreakpoint()
{
    int selected = asm_list->GetSelected();
    if (selected >= 0x8000 && selected <= 0xffff)
    {
        Breakpoint* breakpoint = &debug_data.breakpoint[asm_list->GetSelected()];
        if (*breakpoint == Breakpoint::INACTIVE)
        {
            *breakpoint = Breakpoint::ACTIVE;
            debug_data.breakpoints.insert(selected);
        }
        else
        {
            *breakpoint = Breakpoint::INACTIVE;
            auto iter = debug_data.breakpoints.find(selected);
            debug_data.breakpoints.erase(iter);
        }
    }
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
        text_status->SetText("Status: Game is not running");
        button_attach->SetActive(true);
    }
    else if (!(mapper == 0 || mapper == 3))
    {
        *debug_mode = false;
        text_status->SetText("Status: Unsupported mapper");
        button_attach->SetActive(true);
    }
    else if(machine_status->running == RunningStatus::RUNNING_ROM)
    {
        asm_list->SetSelected(0);
        *debug_mode = true;
        text_status->SetText("Status: Attached");
        button_attach->SetActive(false);
    }
        

}

void Debugger::Detach()
{
    if (*debug_mode == true)
    {
        *debug_mode = false;
        text_status->SetText("Status: Detached");
        button_attach->SetActive(true);
        debug_data.Clear();
        asm_list->cursor = -1;
        asm_list->SetActive(false);
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
            std::string str = "Status: Attached - " + stream.str() + "% of PRG bytes discovered. Signal: " + std::to_string((int)debug_data.signal) + ".";
            text_status->SetText(str);
            last_update = SDL_GetTicks();
            if (debug_data.known_bytes > 100)
            {
                asm_list->SetActive(true);
                asm_list->Update();
            }
            if (debug_data.signal != DebuggerSignal::CLEAR)
            {
                std::string title = "NES Emulator (debugging)";
                SDL_SetWindowTitle(window_main, title.c_str());
            }
        }
    }
    if (last_running_status == RunningStatus::RUNNING_ROM && machine_status->running == RunningStatus::RUNNING)
    {
        logger::PrintLine(logger::LogType::DEBUG, "Auto detached");
        Detach();
    }
    last_running_status = machine_status->running;
}

