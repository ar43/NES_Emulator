#include "user_interface.h"
#include "../../logger/logger.h"
#include "../misc/constants.h"
#include "../../utility/utility.h"
#include "window.h"
#include <SDL_syswm.h>
#include <SDL_ttf.h>

void UserInterface::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) 
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to initialize SDL: " + std::string(SDL_GetError()));
    }

    if (TTF_Init() == -1)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to initialize SDL_TTF: " + std::string(TTF_GetError()));
    }
    font = TTF_OpenFont( "data/Consolas.ttf", 16 );
    if( font == NULL )
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to load font: " + std::string(TTF_GetError()));
    }
}

void UserInterface::Init()
{
    InitSDL();

    window.no_update = true;
    window.Init("NES Emulator", SCREEN_WIDTH * GetScale(), SCREEN_HEIGHT * GetScale(), SCREEN_WIDTH, SCREEN_HEIGHT);
    
    menu_bar.Init(window.GetWindow());
    debugger.Init(font);

    other_windows.push_back(&debugger.window);
}

void UserInterface::UpdateAll()
{
    for (auto window : other_windows)
    {
        window->Update();
    }
}

void UserInterface::SetScale(uint8_t scale)
{
    if (scale < 1 || scale > 6)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Display::SetScale scale too large: " + std::to_string(scale));
        return;
    }
    this->scale = scale;
}

uint8_t UserInterface::GetScale()
{
    return scale;
}

HWND UserInterface::GetSDLWinHandle(SDL_Window* win)
{
    SDL_SysWMinfo infoWindow;
    SDL_VERSION(&infoWindow.version);
    if (!SDL_GetWindowWMInfo(win, &infoWindow))
    {
        return NULL;
    }
    return (infoWindow.info.win.window);
}

std::string UserInterface::GetROMPath(SDL_Window *window)
{
    wchar_t buffer[256] = { 0 };
    wchar_t loc[256] = { 0 };

    size_t len = sizeof(loc); 
    int bytes = GetModuleFileName(NULL, loc, len);
    logger::PrintLine(logger::LogType::DEBUG, utility::ConvertWideToUtf8(loc));
    OPENFILENAME data;
    ZeroMemory(&data, sizeof(data));
    data.lStructSize = sizeof(OPENFILENAME);
    data.hwndOwner = GetSDLWinHandle(window);
    data.lpstrFile = buffer;
    data.nMaxFile = sizeof(buffer);
    data.nFilterIndex = 1;
    if (bytes)
        data.lpstrInitialDir = loc;
    data.lpstrFileTitle = NULL;
    data.lpstrTitle = L"Choose NES Rom";
    data.lpstrFilter = L"NES File\0*.nes\0";
    data.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;

    GetOpenFileName(&data);

    return utility::ConvertWideToUtf8(std::wstring(data.lpstrFile));
}
