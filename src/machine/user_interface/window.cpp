#include "window.h"
#include <SDL.h>
#include <string>
#include "../../logger/logger.h"
#include "button.h"
#include "element.h"
#include "text.h"
#include "checkbox.h"
#include "list.h"
#include "asm_list.h"
#include "debugger.h"
#include "textbox.h"

void Window::Show()
{
    if (!IsShown())
        SDL_ShowWindow(window);
    else
        SDL_RaiseWindow(window);
}

void Window::Toggle()
{
    if (shown)
        Hide();
    else
        Show();
}

Button* Window::AddButton(int x, int y, int w, int h, std::string text, std::function<void()> OnClick)
{
    auto button = std::shared_ptr<Button>(new Button(GetRenderer(), x, y, w, h, text, OnClick));
    elements.push_back(button);
    return button.get();
}

Text *Window::AddText(int x, int y, std::string text, int size)
{
    auto txt = std::shared_ptr<Text>(new Text(GetRenderer(), x, y, text, size));
    elements.push_back(txt);
    return txt.get();
}

void Window::AddCheckbox(int x, int y, std::string text, std::function<void(bool*)> OnClick)
{
    auto checkbox = std::shared_ptr<Checkbox>(new Checkbox(GetRenderer(), x, y, text, OnClick));
    elements.push_back(checkbox);
}

List* Window::AddList(int x, int y, int w, int h)
{
    auto list = std::shared_ptr<List>(new List(GetRenderer(), x, y, w, h));
    elements.push_back(list);
    return list.get();
}

AsmList* Window::AddAsmList(int x, int y, int w, int h, int cursor, DebugData* debug_data)
{
    auto list = std::shared_ptr<AsmList>(new AsmList(GetRenderer(), x, y, w, h, cursor, debug_data));
    elements.push_back(list);
    return list.get();
}

Textbox* Window::AddTextbox(int x, int y, int w, std::string text)
{
    auto textbox = std::shared_ptr<Textbox>(new Textbox(GetRenderer(), x, y, w, text));
    elements.push_back(textbox);
    return textbox.get();
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
            if (OnOpen)
                OnOpen();
            break;

            //Window disappeared
        case SDL_WINDOWEVENT_HIDDEN:
            shown = false;
            current_active_element = -1;
            if(OnClose)
                OnClose();
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
            //current_active_element = -1;
            break;

            //Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            minimized = true;
            //current_active_element = -1;
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
    this->w = width;
    this->h = height;
    //open = true;
}

void Window::HandleEvent(SDL_Event* e)
{
    if (!focus)
        return;
    int i = 0;
    Element* current = nullptr;
    for (auto ele : elements)
    {
        if (!ele->HandleEvent(e, &current_active_element))
            i++;
        if (ele->GetId() == current_active_element)
            current = ele.get();
    }
    if (i == elements.size())
        current_active_element = -1;

    if (SDL_IsTextInputActive()) //if current active element is not textbox, stop text input
    {
        if (current == nullptr)
        {
            SDL_StopTextInput();
        }
        else
        {
            if(Textbox* v = dynamic_cast<Textbox*>(current)) 
            {
            }
            else
            {
                SDL_StopTextInput();
            }
        }
    }
}

void Window::Update()
{
    if (no_update || !shown)
        return;

    //logger::PrintLine(logger::LogType::DEBUG, "Updating window #" + std::to_string(window_id));

    if (OnUpdate)
        OnUpdate();

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
    SDL_RenderClear(renderer);

    if(DrawHook)
        DrawHook(renderer);

    for (auto ele : elements)
    {
        ele->Render(&current_active_element);
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