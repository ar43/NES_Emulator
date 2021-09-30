#include "debugger.h"
#include "../../logger/logger.h"
#include "text.h"
#include "button.h"
#include "../misc/machine_status.h"
#include <string>
#include <iomanip>
#include <sstream>
#include "asm_list.h"
#include "list.h"
#include "textbox.h"
#include "../cpu/register.h"
#include "../../utility/utility.h"
#include <fstream>

void Debugger::Init(SDL_Window* window_main)
{
    this->window_main = window_main;

    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    window.OnOpen = std::bind(&Debugger::Open,this);
    window.OnClose = std::bind(&Debugger::Close,this);
    window.OnUpdate = std::bind(&Debugger::Update, this);
    window.DrawHook = std::bind(&Debugger::DrawBackground, this, std::placeholders::_1);
    
    text_status = window.AddText(3, win_height-12, "Status: Game is not running", 12);

    button_attach = window.AddButton(10,10,0,21,"Attach", std::bind(&Debugger::Attach, this));
    button_continue = window.AddButton(10+button_attach->GetWidth()+30,10,0,21, "Continue", std::bind(&Debugger::Continue, this));
    button_step = window.AddButton(10+button_attach->GetWidth()+30+button_continue->GetWidth()+10,10,0,21, "Step In", std::bind(&Debugger::StepIn, this));
    button_step_over = window.AddButton(10+button_attach->GetWidth()+30+button_continue->GetWidth()+10+button_step->GetWidth()+10,10,0,21, "Step Over", std::bind(&Debugger::StepOver, this));
    button_breakpoint_toggle = window.AddButton(10+button_attach->GetWidth()+30+button_continue->GetWidth()+10+button_step->GetWidth()+10+button_step_over->GetWidth()+10,10,0,21,"Toggle breakpoint", std::bind(&Debugger::ToggleBreakpoint, this, 0));
    
    window.AddCheckbox(win_width-300, 480, "Enable saving and loading data", enable_save_load, std::bind(&Debugger::Checkbox1Click, this, std::placeholders::_1));
    textbox_goto = window.AddTextbox(10, 400, 173, "");
    button_goto = window.AddButton(195, 400, 0, 18, "Go to", std::bind(&Debugger::Goto, this));
    //textbox_bp = window.AddTextbox(100, 700, 173, "");
    button_bp = window.AddButton(195, 420, 0, 18, "Toggle breakpoint", std::bind(&Debugger::ToggleBreakpointText, this));

    //window.AddTextbox(300, 650, 173, "second");
    asm_list = window.AddAsmList(10, 60, 300, 22,-1,&debug_data);

    window.AddText(win_width - 200, 330, "Breakpoints: ", 14);
    list_breakpoints = window.AddList(win_width - 200, 350, 100, 4, &debug_data.breakpoints_draw);
    button_remove_bp = window.AddButton(win_width - 80, 350, 0, 21, "Remove", std::bind(&Debugger::RemoveBreakpoint, this));

    std::stringstream stream;
    text_cycles = window.AddText(win_width - 400, 60, "CPU Data", 14);
    window.AddText(win_width - 400, 60+14+6, "Registers", 14);
    stream << "A: " << std::left << std::setw(3) << "?" << " X: " << std::left << std::setw(3) << "?" << " Y: " << std::left << std::setw(3) << "?" << " P: " << std::left << std::setw(3) << "?" << " SP: " << std::left << std::setw(3) << "?" << " PC: " << std::left << std::setw(4) << "?";
    text_registers = window.AddText(win_width - 400, 60+14*2+6, stream.str(), 14);
    window.AddText(win_width - 400, 60+14*3+6*2, "Flags", 14);
    text_flags = window.AddText(win_width - 400, 60+14*4+6*2, "...", 14);
}

void Debugger::RemoveBreakpoint()
{
    std::string out = list_breakpoints->GetSelectedText();
    if (!out.empty())
    {
        int bp = 0;
        std::string hex_num = out.substr(0, 4);
        std::stringstream ss;
        ss << std::hex << hex_num;
        ss >> bp;
        ToggleBreakpoint(bp);
    }
}

void Debugger::Goto()
{
    if (!textbox_goto->GetText().empty() && textbox_goto->GetText().size() < 8)
    {
        std::string text = textbox_goto->GetText();
        char * p;
        long n = strtoul( text.c_str(), & p, 16 ); 
        if ( * p != 0 ) 
        {  
            SDL_ShowSimpleMessageBox(0, "Input error", "Input a valid hex address", this->window.GetWindow());
        }    
        else 
        {  
            if (n > 0xffff)
                n = 0xffff;
            else if (n < 0)
                n = 0;
            asm_list->InitCursor((int)n, false, true);
            asm_list->Update();
        }
    }
    else
    {
        SDL_ShowSimpleMessageBox(0, "Input error", "Input a valid hex address", this->window.GetWindow());
    }
}

void Debugger::ToggleBreakpointText()
{
    if (!textbox_goto->GetText().empty() && textbox_goto->GetText().size() < 8)
    {
        std::string text = textbox_goto->GetText();
        char * p;
        long n = strtoul( text.c_str(), & p, 16 ); 
        if ( * p != 0 ) 
        {  
            SDL_ShowSimpleMessageBox(0, "Input error", "Input a valid hex address", this->window.GetWindow());
        }    
        else 
        {  
            if (n > 0xffff || n < 0x8000)
                SDL_ShowSimpleMessageBox(0, "Input error", "Input a valid hex address", this->window.GetWindow());
            else
                ToggleBreakpoint((int)n);
        }
    }
    else
    {
        SDL_ShowSimpleMessageBox(0, "Input error", "Input a valid hex address", this->window.GetWindow());
    }
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

void Debugger::ToggleBreakpoint(int loc)
{

    int selected = asm_list->GetSelected();

    if (loc != 0)
        selected = loc;

    if (selected >= 0x8000 && selected <= 0xffff)
    {
        Breakpoint* breakpoint = &debug_data.breakpoint[selected];
        if (*breakpoint == Breakpoint::INACTIVE)
        {
            *breakpoint = Breakpoint::ACTIVE;
            debug_data.breakpoints.insert(selected);
            debug_data.UpdateBreakpoints();
        }
        else
        {
            *breakpoint = Breakpoint::INACTIVE;
            auto iter = debug_data.breakpoints.find(selected);
            debug_data.breakpoints.erase(iter);
            debug_data.UpdateBreakpoints();
        }
        list_breakpoints->SetActive(true);
        list_breakpoints->Update();
        
    }
}

void Debugger::Checkbox1Click(bool* new_state)
{
    /*if (*new_state == true)
    {
        logger::PrintLine(logger::LogType::INFO, "Checked");
    }
    else
    {
        logger::PrintLine(logger::LogType::INFO, "Unchecked");
    }*/

    enable_save_load = *new_state;
}

void Debugger::Attach()
{
    if (machine_status->running == RunningStatus::RUNNING)
    {
        *debug_mode = false;
        text_status->SetText("Status: Game is not running");
        button_attach->SetActive(true);

        button_continue->SetActive(false);
        button_step->SetActive(false);
        button_step_over->SetActive(false);
        button_breakpoint_toggle->SetActive(false);
        button_bp->SetActive(false);
        button_goto->SetActive(false);
;    }
    else if (!(mapper == 0 || mapper == 3))
    {
        *debug_mode = false;
        text_status->SetText("Status: Unsupported mapper");
        button_attach->SetActive(true);

        button_continue->SetActive(false);
        button_step->SetActive(false);
        button_step_over->SetActive(false);
        button_breakpoint_toggle->SetActive(false);
        button_bp->SetActive(false);
        button_goto->SetActive(false);
    }
    else if(machine_status->running == RunningStatus::RUNNING_ROM)
    {
        asm_list->SetSelected(0);
        *debug_mode = true;
        text_status->SetText("Status: Attached");
        button_attach->SetActive(false);

        button_continue->SetActive(true);
        button_step->SetActive(true);
        button_step_over->SetActive(true);
        button_breakpoint_toggle->SetActive(true);
        button_bp->SetActive(true);
        button_goto->SetActive(true);
        LoadData();
    }
        

}

void Debugger::SaveData()
{
    if (!enable_save_load)
        return;

    std::ofstream ofs(std::string(DEBUG_PATH) + "/" + debug_data.md5 + ".dat", std::ios::binary);
    ofs << debug_data.known_bytes << std::endl;
    for(size_t i = 0x8000; i < debug_data.code.size(); i++) 
    {
        if (!debug_data.code[i].empty())
        {
            if(debug_data.is_subroutine[i])
                ofs << "+" << debug_data.code[i] << std::endl;
            else
                ofs << "-" << debug_data.code[i] << std::endl;
        }
        else
        {
            ofs << std::endl;
        }
    }
    logger::PrintLine(logger::LogType::INFO, "Saved debugger data");
}

void Debugger::LoadData()
{
    if (!enable_save_load)
        return;

    std::ifstream ifs(std::string(DEBUG_PATH) + "/" + debug_data.md5 + ".dat", std::ios::binary);
    if (!ifs.is_open())
    {
        return;
    }
    std::string line;
    std::getline(ifs, line);
    debug_data.known_bytes = stoi(line);
    int i = 0x8000;
    while (std::getline(ifs, line))
    {
        if (!line.empty())
        {
            debug_data.code[i] = line.substr(1,line.size());
            if (line[0] == '+')
                debug_data.is_subroutine[i] = true;
        }
        /*else
        {
            logger::PrintLine(logger::LogType::DEBUG, "found empty line");
        }*/
        i++;
    }

    logger::PrintLine(logger::LogType::INFO, "Loaded debugger data");
}

void Debugger::Detach()
{
    if (*debug_mode == true)
    {
        *debug_mode = false;
        SaveData();
        text_status->SetText("Status: Detached");
        button_attach->SetActive(true);
        debug_data.Clear();
        asm_list->cursor = -1;
        asm_list->SetActive(false);
        list_breakpoints->SetActive(false);

        button_continue->SetActive(false);
        button_step->SetActive(false);
        button_step_over->SetActive(false);
        button_breakpoint_toggle->SetActive(false);
        button_bp->SetActive(false);
        button_goto->SetActive(false);
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
    text_cycles->SetText("CPU Data (cycle: " + std::to_string(debug_data.cpu_data.cycles) + ")");
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

void DebugData::UpdateBreakpoints()
{
    breakpoints_draw.clear();
    for (auto i : breakpoints)
    {
        breakpoints_draw.push_back(utility::int_to_hex(i));
    }
}

