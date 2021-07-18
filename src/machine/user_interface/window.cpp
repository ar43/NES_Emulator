#include "window.h"
#include <SDL.h>
#include <string>
#include "../../logger/logger.h"

void Window::HandleEvent(SDL_Event* e, bool *request_exit)
{
    //If an event was detected for this window
    if( e->type == SDL_WINDOWEVENT && e->window.windowID == window_id )
    {
        //Caption update flag
        bool updateCaption = false;
        switch( e->window.event )
        {
            //Window appeared
        case SDL_WINDOWEVENT_SHOWN:
            shown = true;
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

void Window::Init(std::string window_name, int width, int height, int width2, int height2)
{
    window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    window_id = SDL_GetWindowID(window);

    renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to create renderer " + std::string(SDL_GetError()));
    }

    if (SDL_RenderSetLogicalSize(renderer, width2, height2) < 0)
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to set render logical size " + std::string(SDL_GetError()));
    }

    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);
    //open = true;
}

SDL_Window* Window::GetWindow()
{
    return window;
}

SDL_Renderer* Window::GetRenderer()
{
    return renderer;
}