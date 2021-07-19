#include "debugger.h"
#include "../../logger/logger.h"
#include "button.h"

void Debugger::Init()
{
    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    
    auto button_test = std::shared_ptr<Button>(new Button(10,10,30,20));
    window.AddButton(button_test);
}

