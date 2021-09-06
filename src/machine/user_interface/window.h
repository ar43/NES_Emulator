#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>

class Element;
class Button;
class Text;

class Window
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Uint32 window_id;
	
	bool shown = false;
	bool focus = false;
	bool minimized = false;
	SDL_Color color = { 0xf0,0xf0,0xf0 };

	int w, h;

	std::vector<std::shared_ptr<Element>> elements;
public:

	bool no_update = false;

	bool IsFocused() { return focus; }
	bool IsShown() { return shown; }
	bool IsMinimized() { return minimized; }

	void Show() { SDL_ShowWindow(window); }
	void Hide() { SDL_HideWindow(window); }
	void Toggle();

	Button* AddButton(int x, int y, int w, int h, std::string text, std::function<void()> OnClick);
	Text* AddText(int x, int y, std::string text, int size);
	void AddCheckbox(int x, int y, std::string text, std::function<void(bool*)> OnClick);

	void Init(std::string window_name, int width, int height, int rend_width, int rend_height, Uint32 flags = 0);
	void HandleEvent(SDL_Event* e);
	void Update();
	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();
	void HandleWindowEvent(SDL_Event* e, bool *request_exit);
	//void (*DrawHook)(SDL_Renderer* renderer) = nullptr;
	std::function<void(SDL_Renderer*)> DrawHook;
	std::function<void()> OnOpen;
	std::function<void()> OnClose;
	std::function<void()> OnUpdate;
};