#include "debugger.h"
#include "../../logger/logger.h"
#include "text.h"
#include "button.h"
#include "../misc/machine_status.h"
#include <string>
#include <iomanip>
#include <sstream>
#include "asm_list.h"
#include "../cpu/register.h"
#include "../../utility/utility.h"

void Debugger::Init(SDL_Window* window_main)
{
    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    window.OnOpen = std::bind(&Debugger::Open,this);
    window.OnClose = std::bind(&Debugger::Close,this);
    window.OnUpdate = std::bind(&Debugger::Update, this);
    window.DrawHook = std::bind(&Debugger::DrawBackground, this, std::placeholders::_1);
    text_status = window.AddText(3, win_height-12, "Status: Game is not running", 12);
    button_attach = window.AddButton(10,50,73,21,"Attach", std::bind(&Debugger::Attach, this));
    button_breakpoint_toggle = window.AddButton(100,450,173,21,"Toggle breakpoint", std::bind(&Debugger::ToggleBreakpoint, this));
    button_continue = window.AddButton(100,500,173,21, "Continue", std::bind(&Debugger::Continue, this));
    button_step = window.AddButton(100,550,173,21, "Step In", std::bind(&Debugger::StepIn, this));
    button_step_over = window.AddButton(100,600,173,21, "Step Over", std::bind(&Debugger::StepOver, this));
    window.AddCheckbox(100, 51, "Some stuff here", std::bind(&Debugger::Checkbox1Click, this, std::placeholders::_1));
    asm_list = window.AddAsmList(10, 100, 300, 22,-1,&debug_data);
    this->window_main = window_main;

    std::stringstream stream;
    window.AddText(win_width - 400, 100, "CPU Data", 14);
    window.AddText(win_width - 400, 100+14+6, "Registers", 14);
    stream << "A: " << std::left << std::setw(3) << "?" << " X: " << std::left << std::setw(3) << "?" << " Y: " << std::left << std::setw(3) << "?" << " P: " << std::left << std::setw(3) << "?" << " SP: " << std::left << std::setw(3) << "?" << " PC: " << std::left << std::setw(4) << "?";
    text_registers = window.AddText(win_width - 400, 100+14*2+6, stream.str(), 14);
    window.AddText(win_width - 400, 100+14*3+6*2, "Flags", 14);
    text_flags = window.AddText(win_width - 400, 100+14*4+6*2, "...", 14);
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

void Debugger::UpdateCpuData()
{
    std::stringstream stream_register;
    std::string string_flags = "";
    stream_register << "A: " << std::left << std::setw(3) << utility::int_to_hex(debug_data.cpu_data.registers[(int)RegId::A]->get()) << " X: " << std::left << std::setw(3) << utility::int_to_hex(debug_data.cpu_data.registers[(int)RegId::X]->get()) << " Y: " << std::left << std::setw(3) << utility::int_to_hex(debug_data.cpu_data.registers[(int)RegId::Y]->get()) << " P: " << std::left << std::setw(3) << utility::int_to_hex(debug_data.cpu_data.registers[(int)RegId::P]->get()) << " SP: " << std::left << std::setw(3) << utility::int_to_hex(debug_data.cpu_data.registers[(int)RegId::SP]->get()) << " PC: " << std::left << std::setw(4) << utility::int_to_hex(debug_data.cpu_data.registers[(int)RegId::PC]->get());
    text_registers->SetText(stream_register.str());

    for (int i = 0; i < (int)flags::Flags::NUM_FLAGS; i++)
    {
        if ((flags::Flags)i == flags::Flags::UNUSED)
            continue;
        string_flags += flags::to_string((flags::Flags)i) + "=" + std::to_string((int)debug_data.cpu_data.registers[(int)RegId::P]->get_flag((flags::Flags)i));
        string_flags += "  ";
    }
    text_flags->SetText(string_flags);
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
        if (asm_list->IsActive() && debug_data.signal == DebuggerSignal::PAUSE)
        {
            if (debug_data.cpu_data.update)
            {
                debug_data.cpu_data.update = false;
                UpdateCpuData();
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

