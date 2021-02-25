#include "input.h"
#include "../misc/machine_status.h"
#include "../../logger/logger.h"

void Input::Poll(MachineStatus *machine_status, SDL_Window *window)
{
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            logger::PrintLine(logger::LogType::INFO, "Exiting");
			machine_status->running = false;
        }
        else if (e.type == SDL_KEYDOWN) 
        {
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				logger::PrintLine(logger::LogType::INFO, "Exiting");
				machine_status->running = false;
				break;
			}

			case SDLK_F1:
			{
				machine_status->paused = true;
				SDL_SetWindowTitle(window, "Paused");
				break;
			}

			case SDLK_F2:
			{
				bool* pnt = machine_status->speedup;
				*pnt = !*pnt;
				break;
			}

			case SDLK_F3:
			{
				machine_status->reset = 1;
				break;
			}

			default:
				break;
			}
        }

    }
}

void Input::PollPause(MachineStatus *machine_status)
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			logger::PrintLine(logger::LogType::INFO, "Exiting");
			machine_status->running = false;
		}
		else if (e.type == SDL_KEYDOWN) 
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				logger::PrintLine(logger::LogType::INFO, "Exiting");
				machine_status->running = false;
				break;
			}

			case SDLK_F1:
			{
				machine_status->paused = false;
				break;
			}

			case SDLK_F3:
			{
				machine_status->reset = 1;
				break;
			}

			default:
				break;
			}
		}

	}
	SDL_Delay(10);
}