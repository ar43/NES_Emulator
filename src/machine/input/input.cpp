#include "input.h"
#include "../misc/machine_status.h"
#include "../../logger/logger.h"
#include "../ppu/display/display.h"
#include "../user_interface/menu_bar.h"
#include "../../utility/utility.h"
#include "../user_interface/user_interface.h"
#include <SDL_syswm.h>

Input::Input()
{
	keymaps[0][(int)JoypadButtons::A] = SDLK_x;
	keymaps[0][(int)JoypadButtons::B] = SDLK_y;
	keymaps[0][(int)JoypadButtons::RIGHT] = SDLK_RIGHT;
	keymaps[0][(int)JoypadButtons::LEFT] = SDLK_LEFT;
	keymaps[0][(int)JoypadButtons::DOWN] = SDLK_DOWN;
	keymaps[0][(int)JoypadButtons::UP] = SDLK_UP;
	keymaps[0][(int)JoypadButtons::SELECT] = SDLK_BACKSPACE;
	keymaps[0][(int)JoypadButtons::START] = SDLK_RETURN;

	keymaps[1][(int)JoypadButtons::A] = SDLK_k;
	keymaps[1][(int)JoypadButtons::B] = SDLK_j;
	keymaps[1][(int)JoypadButtons::RIGHT] = SDLK_d;
	keymaps[1][(int)JoypadButtons::LEFT] = SDLK_a;
	keymaps[1][(int)JoypadButtons::DOWN] = SDLK_s;
	keymaps[1][(int)JoypadButtons::UP] = SDLK_w;
	keymaps[1][(int)JoypadButtons::SELECT] = SDLK_i;
	keymaps[1][(int)JoypadButtons::START] = SDLK_u;
}

void Input::Poll(MachineStatus *machine_status, UserInterface *ui)
{
    while (SDL_PollEvent(&e) != 0)
    {
		bool request_exit = false;

		if (e.type == SDL_WINDOWEVENT)
		{
			ui->window.HandleWindowEvent(&e, &request_exit);
			ui->debugger.window.HandleWindowEvent(&e);
			ui->controls.window.HandleWindowEvent(&e);
			ui->about.window.HandleWindowEvent(&e);
		}
		else
		{
			ui->debugger.window.HandleEvent(&e);
			ui->controls.window.HandleEvent(&e);
			ui->about.window.HandleEvent(&e);
		}

        if (e.type == SDL_QUIT || request_exit)
        {
            logger::PrintLine(logger::LogType::INFO, "Exiting");
			machine_status->running = RunningStatus::NOT_RUNNING;
        }
        else if (e.type == SDL_KEYDOWN && ui->window.IsFocused()) 
        {
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				logger::PrintLine(logger::LogType::INFO, "Exiting");
				machine_status->running = RunningStatus::NOT_RUNNING;
			}
			else if (e.key.keysym.sym == SDLK_F1)
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
			}
			else if (e.key.keysym.sym == SDLK_F2)
			{
				bool* pnt = machine_status->speedup;
				*pnt = !*pnt;
			}
			else if (e.key.keysym.sym == SDLK_F3)
			{
				machine_status->reset = ResetType::NORMAL;
			}
			else if (e.key.keysym.sym == SDLK_F4)
			{
				machine_status->force_render = !machine_status->force_render;
			}
			else if (e.key.keysym.sym == SDLK_F5)
			{
				if (!(*machine_status->mute))
				{
					*machine_status->mute = true;
				}
				else
				{
					*machine_status->mute = false;
				}
			}
			else
			{
				bool out = false;
				for (int j = 0; j < 2; j++)
				{
					for (int i = 0; i < Joypad::num_keys; i++)
					{
						if (e.key.keysym.sym == keymaps[j][i])
						{
							joypad[j].SetState((JoypadButtons)i);
							out = true;
							break;
						}
					}
					if (out)
						break;
				}
			}
        }
		else if (e.type == SDL_KEYUP && ui->window.IsFocused())
		{
			bool out = false;
			for (int j = 0; j < 2; j++)
			{
				for (int i = 0; i < Joypad::num_keys; i++)
				{
					if (e.key.keysym.sym == keymaps[j][i])
					{
						joypad[j].ResetState((JoypadButtons)i);
						out = true;
						break;
					}
				}
				if (out)
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
			ui->debugger.window.Show();
			break;
		}
		case (WORD)MenuBarID::CONTROLS:
		{
			ui->controls.window.Show();
			break;
		}
		case (WORD)MenuBarID::ABOUT:
		{
			ui->about.window.Show();
			break;
		}
		default: break;
	}
}