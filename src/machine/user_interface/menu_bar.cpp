#include "menu_bar.h"
#include <SDL_syswm.h>
#include "../../utility/utility.h"
#include "../../logger/logger.h"
#include "../user_interface/user_interface.h"

void MenuBar::Init(SDL_Window* win)
{
    Activate(UserInterface::GetSDLWinHandle(win));
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    AdjustWindowSize(win);
}

void MenuBar::AdjustWindowSize(SDL_Window* win)
{
    int w, h = 0;
    MENUBARINFO minfo;
    minfo.cbSize = sizeof(MENUBARINFO);
    if (!GetMenuBarInfo(UserInterface::GetSDLWinHandle(win), OBJID_MENU, 0, &minfo))
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Cant get menu info");

    int diff = minfo.rcBar.bottom - minfo.rcBar.top;
    logger::PrintLine(logger::LogType::DEBUG, "Menu bar size: " + std::to_string(diff));

    SDL_GetWindowSize(win, &w,&h);
    SDL_SetWindowSize(win, w, h+diff+1); //off by one error without +1.... strange
}

void MenuBar::Activate(HWND windowRef)
{
    hMenuBar = CreateMenu();
    hGame = CreateMenu();
    hFile = CreateMenu();
    hOptions = CreateMenu();
    hTools = CreateMenu();
    hHelp = CreateMenu();

    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hGame, L"Game");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hOptions, L"Options");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hTools, L"Tools");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, L"Help");

    AppendMenu(hGame, MF_STRING, (UINT_PTR)MenuBarID::PAUSE, L"Pause");
    AppendMenu(hGame, MF_STRING, (UINT_PTR)MenuBarID::RESET, L"Reset");
    AppendMenu(hGame, MF_SEPARATOR, 0, NULL);
    AppendMenu(hGame, MF_STRING, (UINT_PTR)MenuBarID::POWER_OFF, L"Power OFF");
    

    AppendMenu(hFile, MF_STRING, (UINT_PTR)MenuBarID::LOAD_ROM, L"Open");
    AppendMenu(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFile, MF_STRING, (UINT_PTR)MenuBarID::EXIT, L"Exit");

    AppendMenu(hOptions, MF_STRING, (UINT_PTR)MenuBarID::CONTROLS, L"Controls");

    AppendMenu(hTools, MF_STRING, (UINT_PTR)MenuBarID::DEBUG, L"Debug");

    AppendMenu(hHelp, MF_STRING, (UINT_PTR)MenuBarID::ABOUT, L"About");

    SetMenu(windowRef, hMenuBar);
}