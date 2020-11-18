// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_WINDOW_H
#define GAMMA_WINDOW_H
#include "../Gamma.h"
#include <SDL.h>
#include <stdint.h>
#include <queue>

namespace Gamma {

	enum class FullscreenState {
		WINDOWED = 0,
		WINDOWED_FULLSCREEN = SDL_WINDOW_FULLSCREEN,
		FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP
	};

	enum class WindowState {
		CLOSED = false,
		OPEN = true,
		HIDDEN = SDL_WINDOW_HIDDEN,
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
		bool ShouldClose(void);
	private:
		WindowClass WndClass;
		SDL_Window* pWindow;
		std::queue<WindowEvent> EventQueue;
		friend class Renderer;
	};

}

#endif