#include "controls.h"
#include "../input/joypad.h"
#include "../../logger/logger.h"

void Controls::Init(SDL_Window* window_main)
{
    window.Init("Controls", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    window.OnOpen = std::bind(&Controls::Open,this);
    window.OnClose = std::bind(&Controls::Close,this);
    window.OnUpdate = std::bind(&Controls::Update, this);

    int offset = 0;
    window.AddText(35, 10, "Joypad 0 controls:", 14);
    for (int i = 0; i < Joypad::num_keys; i++)
    {
        window.AddText(35, 40 + i * 30 + 4, Joypad::ButtonToString((JoypadButtons)i), 14);
        button_keymap[0][i] = window.AddButton(95, 40 + i * 30, 100, 21, std::string(SDL_GetKeyName(keymaps[0][i])), std::bind(&Controls::OnButtonClick, this, 0, i));
        offset = 10 + i * 30;
    }

    window.AddText(35, offset+100, "Joypad 1 controls:", 14);
    for (int i = 0; i < Joypad::num_keys; i++)
    {
        window.AddText(35, offset + 130 + i * 30 + 4, Joypad::ButtonToString((JoypadButtons)i), 14);
        button_keymap[1][i] = window.AddButton(95, offset + 130 + i * 30, 100, 21, std::string(SDL_GetKeyName(keymaps[1][i])), std::bind(&Controls::OnButtonClick, this, 1, i));
    }
}

void Controls::OnButtonClick(int joynum, int butnum)
{
    logger::PrintLine(logger::LogType::DEBUG, "Pressed: " + std::to_string(joynum) + " " + std::to_string(butnum));
}

void Controls::Open()
{
}

void Controls::Close()
{
}

void Controls::Update()
{
}
