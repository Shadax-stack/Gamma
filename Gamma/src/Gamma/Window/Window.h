// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_WINDOW_H
#define GAMMA_WINDOW_H
#include "../Gamma.h"
#include <SDL.h>
#include <stdint.h>
#include <vector>

namespace Gamma {

	//Pre-declare any freind classes we may be using
	namespace OpenGL {
		class Renderer;
	}

	namespace Direct3D9 {
		class Renderer;
	}

	enum class FullscreenState {
		WINDOWED = 0,
		FULLSCREEN_WINDOWED = SDL_WINDOW_FULLSCREEN,
		FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP
	};

	enum class WindowState {
		CLOSED = 0,
		OPEN = SDL_WINDOW_SHOWN,
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
		~Window(void);
		void OpenWindow(WindowClass wndclass, const char* title, int32_t width, int32_t height, int32_t xpos = 0, int32_t ypos = 30);
		void CloseWindow(void);
		void PollEvents(void);
		WindowState GetState(void);
		bool ShouldClose(void);
	private:
		WindowClass WndClass;
		SDL_Window* pWindow;
		std::vector<WindowEvent> EventQueue;
		int Width, Height;
		friend class OpenGL::Renderer;
		friend class Direct3D9::Renderer;
	};

}

#endif