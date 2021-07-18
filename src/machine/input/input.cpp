#include "input.h"
#include "../misc/machine_status.h"
#include "../../logger/logger.h"
#include "../ppu/display/display.h"
#include "../user_interface/menu_bar.h"
#include "../../utility/utility.h"
#include "../user_interface/user_interface.h"
#include <SDL_syswm.h>

void Input::Poll(MachineStatus *machine_status, UserInterface *ui)
{
    while (SDL_PollEvent(&e) != 0)
    {
		bool request_exit = false;

		ui->window.HandleEvent(&e, &request_exit);
		ui->debugger.window.HandleEvent(&e, nullptr);

        if (e.type == SDL_QUIT || request_exit)
        {
            logger::PrintLine(logger::LogType::INFO, "Exiting");
			machine_status->running = RunningStatus::NOT_RUNNING;
        }
        else if (e.type == SDL_KEYDOWN) 
        {
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			{
				logger::PrintLine(logger::LogType::INFO, "Exiting");
				machine_status->running = RunningStatus::NOT_RUNNING;
				break;
			}

			case SDLK_RETURN:
			{
				joypad[0].SetState(JoypadButtons::START);
				break;
			}
			case SDLK_BACKSPACE:
			{
				joypad[0].SetState(JoypadButtons::SELECT);
				break;
			}
			case SDLK_UP:
			{
				joypad[0].SetState(JoypadButtons::UP);
				break;
			}
			case SDLK_DOWN:
			{
				joypad[0].SetState(JoypadButtons::DOWN);
				break;
			}
			case SDLK_LEFT:
			{
				joypad[0].SetState(JoypadButtons::LEFT);
				break;
			}
			case SDLK_RIGHT:
			{
				joypad[0].SetState(JoypadButtons::RIGHT);
				break;
			}
			case SDLK_y:
			{
				joypad[0].SetState(JoypadButtons::B);
				break;
			}
			case SDLK_x:
			{
				joypad[0].SetState(JoypadButtons::A);
				break;
			}

			case SDLK_F1:
			{
				if (machine_status->paused == false)
				{
					machine_status->paused = true;
					SDL_SetWindowTitle(ui->window.GetWindow(), "NES Emulator (paused)");
				}
				else
				{
					machine_status->paused = false;
					joypad[0].Clear();
					joypad[1].Clear();
				}
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
				machine_status->reset = ResetType::NORMAL;
				break;
			}

			case SDLK_F4:
			{
				machine_status->force_render = !machine_status->force_render;
				break;
			}

			case SDLK_F5:
			{
				if (!(*machine_status->mute))
				{
					*machine_status->mute = true;
				}
				else
				{
					*machine_status->mute = false;
				}
				break;
			}

			default:
				break;
			}
        }
		else if (e.type == SDL_KEYUP)
		{
			switch (e.key.keysym.sym)
			{

			case SDLK_RETURN:
			{
				joypad[0].ResetState(JoypadButtons::START);
				break;
			}
			case SDLK_BACKSPACE:
			{
				joypad[0].ResetState(JoypadButtons::SELECT);
				break;
			}
			case SDLK_UP:
			{
				joypad[0].ResetState(JoypadButtons::UP);
				break;
			}
			case SDLK_DOWN:
			{
				joypad[0].ResetState(JoypadButtons::DOWN);
				break;
			}
			case SDLK_LEFT:
			{
				joypad[0].ResetState(JoypadButtons::LEFT);
				break;
			}
			case SDLK_RIGHT:
			{
				joypad[0].ResetState(JoypadButtons::RIGHT);
				break;
			}
			case SDLK_y:
			{
				joypad[0].ResetState(JoypadButtons::B);
				break;
			}
			case SDLK_x:
			{
				joypad[0].ResetState(JoypadButtons::A);
				break;
			}

			default:
				break;
			}
		}
		else if (e.type == SDL_SYSWMEVENT)
		{
			if (e.syswm.msg->msg.win.msg == WM_COMMAND)
			{
				HandleMenuBar(machine_status, ui, LOWORD(e.syswm.msg->msg.win.wParam));
			}
		}

    }
}

void Input::HandleMenuBar(MachineStatus *machine_status, UserInterface* ui, WORD param)
{
	switch (param)
	{
		case (WORD)MenuBarID::EXIT:
		{
			logger::PrintLine(logger::LogType::INFO, "Exiting");
			machine_status->running = RunningStatus::NOT_RUNNING;
			break;
		}
		case (WORD)MenuBarID::LOAD_ROM:
		{
			machine_status->pending_rom = UserInterface::GetROMPath(ui->window.GetWindow());

			if (!machine_status->pending_rom.empty())
			{
				logger::PrintLine(logger::LogType::INFO, "Attempting to Open ROM: " + machine_status->pending_rom);
				machine_status->running = RunningStatus::RUNNING;
			}
			break;
		}
		case (WORD)MenuBarID::PAUSE:
		{
			machine_status->paused = !machine_status->paused;
			if(machine_status->paused)
				SDL_SetWindowTitle(ui->window.GetWindow(), "NES Emulator (paused)");
			break;
		}
		case (WORD)MenuBarID::RESET:
		{
			machine_status->reset = ResetType::NORMAL;
			break;
		}
		case (WORD)MenuBarID::POWER_OFF:
		{
			machine_status->running = RunningStatus::RUNNING;
			break;
		}
		case (WORD)MenuBarID::DEBUG:
		{
			ui->debugger.window.Toggle();
			break;
		}
		default: break;
	}
}