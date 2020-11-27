// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Window.h"
#include "../Core/Log.h"
#include <iostream>
#include <vector>
#ifdef GAMMA_GRAPHICS_API_VULKAN
#include <SDL_vulkan.h>
#endif

namespace Gamma {

	WindowClass::WindowClass(void) : FullscreenState(FullscreenState::WINDOWED), WindowState(WindowState::OPEN) {}

	uint32_t WindowClass::GetFlags(void) {
		uint32_t Flags = 0;
		switch (FullscreenState) {
		case FullscreenState::FULLSCREEN:
			Flags |= SDL_WINDOW_FULLSCREEN;
			break;
		case FullscreenState::FULLSCREEN_WINDOWED:
			Flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			break;
		default:
			break;
		}
		switch (WindowState) {
		case WindowState::HIDDEN:
			Flags |= SDL_WINDOW_HIDDEN;
			break;
		case WindowState::OPEN:
			Flags |= SDL_WINDOW_SHOWN;
			break;
		case WindowState::MINIMIZED:
			Flags |= SDL_WINDOW_MINIMIZED;
		default:
			break;
		}
		#ifdef GAMMA_GRAPHICS_API_VULKAN
		Flags |= SDL_WINDOW_VULKAN;
		#endif
		return Flags;
	}

	WindowEvent::WindowEvent(const SDL_Event event) : EventData(event) {}

	Window::~Window(void) {
		//We need this destructor so that program does not crash at the end when objects are being freed for some reason
	}

	void Window::OpenWindow(WindowClass wndclass, const char* title, int32_t width, int32_t height, int32_t xpos, int32_t ypos) {
		WndClass = wndclass;
		bool wn1 = width == -1, hn1 = height == -1;
		if (wn1||hn1) {
			SDL_DisplayMode DisplayMode;
			SDL_GetCurrentDisplayMode(0 /* Primary Monitor */, &DisplayMode);
			if (wn1) {
				width = DisplayMode.w;
			}
			if (hn1) {
				height = DisplayMode.h;
			}
		}
		InternalWindow = SDL_CreateWindow(title, xpos, ypos, width, height, WndClass.GetFlags());
		GAMMA_ASSERT(InternalWindow, "Fatal error in creating window: %s", SDL_GetError());
		WindowID = SDL_GetWindowID(InternalWindow);
		GAMMA_INFO("WINDOW", "Created window %i of size %i by %i", WindowID, width, height);
		Dimensions.Width = (int)width;
		Dimensions.Height = (int)height;
	}

	void Window::CloseWindow(void) {
		GAMMA_INFO("WINDOW", "Destroying window %i", WindowID);
		SDL_DestroyWindow(InternalWindow);
	}

	WindowState Window::GetState(void) {
		return WndClass.WindowState;
	}

	void Window::PollEvents(void) {
		SDL_Event Event;
		std::vector<SDL_Event> IrrelevantEvents;
		IrrelevantEvents.reserve(2048);
		while (SDL_PollEvent(&Event)) {
			if (Event.window.windowID == WindowID) {
				switch (Event.window.event) {
					case SDL_WINDOWEVENT_CLOSE: {
						WndClass.WindowState = WindowState::CLOSED;
					} break; 
					default: {
						EventQueue.emplace_back(Event);
					} break;
				}
			} else {
				IrrelevantEvents.emplace_back(Event);
			}
		}
		for (SDL_Event& IrrelevantEvent : IrrelevantEvents) {
			SDL_PushEvent(&IrrelevantEvent);
		}
	}

	bool Window::ShouldClose(void) {
		return GetState() == WindowState::CLOSED;
	}

	bool Window::GetFullscreenState(void) {
		return (WndClass.FullscreenState == FullscreenState::FULLSCREEN) || (WndClass.FullscreenState == FullscreenState::FULLSCREEN_WINDOWED);
	}

	SDL_Window* Window::GetInternalWindow(void) {
		return InternalWindow;
	}

	Rectangle Window::GetDimensions(void) {
		return Dimensions;
	}

	WindowClass Window::GetInternalWindowClass(void) {
		return WndClass;
	}

	void Window::SetFullscreenState(FullscreenState fsstate) {
		int Result;
		WndClass.FullscreenState = fsstate;
		switch (fsstate) {
		case FullscreenState::WINDOWED:
			Result = SDL_SetWindowFullscreen(InternalWindow, 0);
			GAMMA_ASSERT_CRITICAL(Result == 0, "WINDOW", "Unable to set window to windowed mode: %s", SDL_GetError());
			break;
		case FullscreenState::FULLSCREEN_WINDOWED:
			Result = SDL_SetWindowFullscreen(InternalWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			GAMMA_ASSERT_CRITICAL(Result == 0, "WINDOW", "Unable to set window to fullscreen windowed mode: %s", SDL_GetError());
			break;
		case FullscreenState::FULLSCREEN:
			Result = SDL_SetWindowFullscreen(InternalWindow, SDL_WINDOW_FULLSCREEN);
			GAMMA_ASSERT_CRITICAL(Result == 0, "WINDOW", "Unable to set window to fullscreen mode: %s", SDL_GetError());
			break;
		default:
			break;
		}
		SDL_GetWindowSize(InternalWindow, &Dimensions.Width, &Dimensions.Height);
	}

}