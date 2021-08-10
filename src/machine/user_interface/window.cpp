#include "window.h"
#include <SDL.h>
#include <string>
#include "../../logger/logger.h"
#include "button.h"
#include "element.h"
#include "text.h"

void Window::Toggle()
{
    if (shown)
        Hide();
    else
        Show();
}

void Window::AddButton(int x, int y, int w, int h, std::string text, TTF_Font *font)
{
    auto button = std::shared_ptr<Button>(new Button(GetRenderer(),x,y,w,h,text, font));
    elements.push_back(button);
}

void Window::AddText(int x, int y, std::string text, TTF_Font *font)
{
    auto txt = std::shared_ptr<Text>(new Text(GetRenderer(),x,y,text, font));
    elements.push_back(txt);
}

void Window::HandleWindowEvent(SDL_Event* e, bool *request_exit)
{
    //If an event was detected for this window
    if(e->window.windowID == window_id )
    {
        //Caption update flag
        bool updateCaption = false;
        switch( e->window.event )
        {
            //Window appeared
        case SDL_WINDOWEVENT_SHOWN:
            shown = true;
            Update();
            break;

            //Window disappeared
        case SDL_WINDOWEVENT_HIDDEN:
            shown = false;
            break;

            //Repaint on expose
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(renderer);
            break;

            //    //Mouse enter
            //case SDL_WINDOWEVENT_ENTER:
            //    mMouseFocus = true;
            //    break;

            //    //Mouse exit
            //case SDL_WINDOWEVENT_LEAVE:
            //    mMouseFocus = false;
            //    break;

            //Keyboard focus gained
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            focus = true;
            logger::PrintLine(logger::LogType::DEBUG, "Focus ON for window id " + std::to_string(window_id));
            break;

            //Keyboard focus lost
        case SDL_WINDOWEVENT_FOCUS_LOST:
            focus = false;
            logger::PrintLine(logger::LogType::DEBUG, "Focus OFF for window id " + std::to_string(window_id));
            break;

            //Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            minimized = true;
            break;

            //Window maxized
        case SDL_WINDOWEVENT_MAXIMIZED:
            minimized = false;
            break;

            //Window restored
        case SDL_WINDOWEVENT_RESTORED:
            minimized = false;
            break;

        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow( window );
            if (request_exit != nullptr)
                *request_exit = true;
            break;

        }
    }
}

void Window::Init(std::string window_name, int width, int height, int rend_width, int rend_height, Uint32 flags)
{
    window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    window_id = SDL_GetWindowID(window);

    renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to create renderer " + std::string(SDL_GetError()));
    }

    if (SDL_RenderSetLogicalSize(renderer, rend_width, rend_height) < 0)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to set render logical size " + std::string(SDL_GetError()));
    }

    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);
    //open = true;
}

void Window::HandleEvent(SDL_Event* e)
{
    if (!focus)
        return;
    for (auto ele : elements)
    {
        ele->HandleEvent(e);
    }
}

void Window::Update()
{
    if (no_update || !shown)
        return;

    //logger::PrintLine(logger::LogType::DEBUG, "Updating window #" + std::to_string(window_id));

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderClear(renderer);

    for (auto ele : elements)
    {
        ele->Render(renderer);
    }

    SDL_RenderPresent(renderer);
}

SDL_Window* Window::GetWindow()
{
    return window;
}

SDL_Renderer* Window::GetRenderer()
{
    return renderer;
}