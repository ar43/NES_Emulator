#include "user_interface.h"
#include "../../logger/logger.h"
#include "../misc/constants.h"

SDL_Window* UserInterface::GetWindow()
{
    return main_window;
}

void UserInterface::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0) 
    {
        logger::PrintLine(logger::LogType::FATAL_ERROR, "Unable to initialize SDL: " + std::string(SDL_GetError()));
    }

    main_window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH*GetScale(), SCREEN_HEIGHT*GetScale(), 0);
    menu_bar.Init(main_window);
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
