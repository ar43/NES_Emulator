#include "debugger.h"
#include "../../logger/logger.h"
#include "button.h"

void Debugger::Init(TTF_Font *font)
{
    window.Init("Debugger", win_width, win_height, win_width, win_height, SDL_WINDOW_HIDDEN);
    
    window.AddText(50, 10, "Debugger", font);
    window.AddButton(10,50,73,21,"Okay", font);
}

