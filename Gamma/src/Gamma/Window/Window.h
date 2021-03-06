// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#ifndef GAMMA_WINDOW_H
#define GAMMA_WINDOW_H
#include "../Gamma.h"
#include <SDL.h>
#include <stdint.h>
#include <vector>
#include <Krypton/Vector.h>

namespace Gamma {

	//Pre-declare any freind classes we may be using
	namespace Graphics {
		class Renderer;
	}

	// A rectangle struct, TODO: move this to Krypton

	struct Rectangle {
		int Width;
		int Height;
	};

	enum class FullscreenState {
		WINDOWED = 0,
		FULLSCREEN_WINDOWED = SDL_WINDOW_FULLSCREEN,
		FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP
	};

	enum class WindowState {
		CLOSED = 0,
		OPEN = SDL_WINDOW_SHOWN,
		HIDDEN = SDL_WINDOW_HIDDEN,
		MINIMIZED = SDL_WINDOW_MINIMIZED
	};

	struct GAMMA_API WindowClass {
	public:
		//Initialize the window class members to a certain default value
		WindowClass(void);
		//Basic concped is based off the WINAPI window class
		FullscreenState FullscreenState;
		WindowState WindowState;
		uint32_t GetFlags(void);
	};

	struct GAMMA_API WindowEvent {
	public:
		WindowEvent(const SDL_Event event);
		SDL_Event EventData;
	};

	class GAMMA_API Window {
	public:
		~Window(void);
		void OpenWindow(WindowClass wndclass, const char* title, int32_t width = -1, int32_t height = -1, int32_t xpos = 0, int32_t ypos = 30);
		void CloseWindow(void);
		void PollEvents(void);
		WindowState GetState(void);
		bool ShouldClose(void);
		bool GetFullscreenState(void);
		void SetFullscreenState(FullscreenState fsstate);
		SDL_Window* GetInternalWindow(void);
		Rectangle GetDimensions(void);
		WindowClass GetInternalWindowClass(void);
	private:
		WindowClass WndClass;
		SDL_Window* InternalWindow;
		uint32_t WindowID;
		std::vector<WindowEvent> EventQueue;
		Rectangle Dimensions;
		friend class Graphics::Renderer;
	};

}

#endif