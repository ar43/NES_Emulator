#include "debugger.h"
#include "../../logger/logger.h"

void Debugger::Init()
{
    window.Init("Debugger", win_width, win_height, win_width, win_height);
    SDL_HideWindow(window.GetWindow());
}

