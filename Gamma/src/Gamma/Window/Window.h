#ifndef GAMMA_WINDOW_H
#define GAMMA_WINDOW_H
#include "../Gamma.h"
#include <SDL.h>
#include <stdint.h>
#include <queue>

namespace Gamma {

	enum FullscreenState {
		GAMMA_FULLSCREEN_STATE_WINDOWED = 0,
		GAMMA_FULLSCREEN_STATE_WINDOWED_FULLSCREEN = SDL_WINDOW_FULLSCREEN,
		GAMMA_FULLSCREEN_STATE_FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP
	};

	enum WindowState {
		GAMMA_WINDOW_STATE_CLOSED = false,
		GAMMA_WINDOW_STATE_OPEN = true,
		GAMMA_WINDOW_STATE_HIDDEN,
	};

	struct GAMMA_API WindowClass {
	public:
		//Initialize the window class members to a certain default value
		WindowClass(void);
		//Basic concped is based off the WINAPI window class
		FullscreenState FullscreenState;
		WindowState WindowState;

	};

	struct GAMMA_API WindowEvent {
	public:
		WindowEvent(const SDL_Event event);
		SDL_Event EventData;
	};

	class GAMMA_API Window {
	public:
		void OpenWindow(WindowClass wndclass, const char* title, int32_t width, int32_t height, int32_t xpos = 0, int32_t ypos = 30);
		void CloseWindow(void);
		void PollEvents(void);
		WindowState GetState(void);
	private:
		WindowClass WndClass;
		SDL_Window* pWindow;
		std::queue<WindowEvent> EventQueue;
		friend class Renderer;
	};

}

#endif