#include "menu_bar.h"
#include <SDL_syswm.h>
#include "../../utility/utility.h"
#include "../../logger/logger.h"

void MenuBar::Init(SDL_Window* win)
{
    Activate(GetSDLWinHandle(win));
    SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
    AdjustWindowSize(win);
}

void MenuBar::AdjustWindowSize(SDL_Window* win)
{
    int w, h = 0;
    MENUBARINFO minfo;
    minfo.cbSize = sizeof(MENUBARINFO);
    if (!GetMenuBarInfo(GetSDLWinHandle(win), OBJID_MENU, 0, &minfo))
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Cant get menu info");

    int diff = minfo.rcBar.bottom - minfo.rcBar.top;
    logger::PrintLine(logger::LogType::DEBUG, "Menu bar size: " + std::to_string(diff));

    SDL_GetWindowSize(win, &w,&h);
    SDL_SetWindowSize(win, w, h+diff+1); //off by one error without +1.... strange
}

HWND MenuBar::GetSDLWinHandle(SDL_Window* win)
{
    SDL_SysWMinfo infoWindow;
    SDL_VERSION(&infoWindow.version);
    if (!SDL_GetWindowWMInfo(win, &infoWindow))
    {
        return NULL;
    }
    return (infoWindow.info.win.window);
}


void MenuBar::Activate(HWND windowRef)
{
    hMenuBar = CreateMenu();
    hGame = CreateMenu();
    hFile = CreateMenu();
    hOptions = CreateMenu();
    hTools = CreateMenu();
    hHelp = CreateMenu();

    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, utility::CharToWString("File").c_str());
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hGame, utility::CharToWString("Game").c_str());
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hOptions, utility::CharToWString("Options").c_str());
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hTools, utility::CharToWString("Tools").c_str());
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, utility::CharToWString("Help").c_str());

    AppendMenu(hGame, MF_STRING, (UINT_PTR)MenuBarID::PAUSE, utility::CharToWString("Pause").c_str());
    AppendMenu(hGame, MF_STRING, (UINT_PTR)MenuBarID::RESET, utility::CharToWString("Reset").c_str());
    

    AppendMenu(hFile, MF_STRING, (UINT_PTR)MenuBarID::LOADROM, utility::CharToWString("Open").c_str());
    AppendMenu(hFile, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFile, MF_STRING, (UINT_PTR)MenuBarID::EXIT, utility::CharToWString("Exit").c_str());

    AppendMenu(hOptions, MF_STRING, (UINT_PTR)MenuBarID::CONTROLS, utility::CharToWString("Controls").c_str());

    AppendMenu(hTools, MF_STRING, (UINT_PTR)MenuBarID::DEBUG, utility::CharToWString("Debug").c_str());

    AppendMenu(hHelp, MF_STRING, (UINT_PTR)MenuBarID::ABOUT, utility::CharToWString("About").c_str());

    SetMenu(windowRef, hMenuBar);
}