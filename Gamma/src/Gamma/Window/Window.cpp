// Copyright (c) 2020 Shadax-stack <shadax32@gmail.com>
// This file is subject to the terms and conditions defined in
// file 'LICENSE.txt', which is part of this source code package.
#include "Window.h"
#include "../Core/Log.h"
#include <iostream>
#include <vector>

#define WINDOW_GRAPHICS_API_FLAG SDL_WINDOW_OPENGL

namespace Gamma {

	WindowClass::WindowClass(void) : FullscreenState(FullscreenState::WINDOWED), WindowState(WindowState::OPEN) {}

	WindowEvent::WindowEvent(const SDL_Event event) : EventData(event) {}

	Window::~Window(void) {
		//We need this destructor so that program does not crash at the end when objects are being freed for some reason
	}

	void Window::OpenWindow(WindowClass wndclass, const char* title, int32_t width, int32_t height, int32_t xpos, int32_t ypos) {
		WndClass = wndclass;
		pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, WINDOW_GRAPHICS_API_FLAG);
		GAMMA_ASSERT(pWindow, "Fatal error in creating window: %s", SDL_GetError());
		GAMMA_INFO("Created window %i of size %i by %i", SDL_GetWindowID(pWindow), width, height);
		
		Dimensions.Width = (int)width;
		Dimensions.Height = (int)height;
	}

	void Window::CloseWindow(void) {
		GAMMA_INFO("Destroying window %i", SDL_GetWindowID(pWindow));
		SDL_DestroyWindow(pWindow);
	}

	WindowState Window::GetState(void) {
		return WndClass.WindowState;
	}

	void Window::PollEvents(void) {
		SDL_Event Event;
		std::vector<SDL_Event> IrrelevantEvents;
		IrrelevantEvents.reserve(2048);
		while (SDL_PollEvent(&Event)) {
			if (Event.window.windowID == SDL_GetWindowID(pWindow)) {
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
		return pWindow;
	}

	Rectangle Window::GetDimensions(void) {
		return Dimensions;
	}

}