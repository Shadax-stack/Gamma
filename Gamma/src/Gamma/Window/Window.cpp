#include "Window.h"
#include <iostream>
#include <vector>

namespace Gamma {

	WindowClass::WindowClass(void) : FullscreenState(GAMMA_FULLSCREEN_STATE_WINDOWED), WindowState(GAMMA_WINDOW_STATE_OPEN) {}

	WindowEvent::WindowEvent(const SDL_Event event) : EventData(event) {}

	void Window::OpenWindow(WindowClass wndclass, const char* title, int32_t width, int32_t height, int32_t xpos, int32_t ypos) {
		WndClass = wndclass;
		pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
	}

	void Window::CloseWindow(void) {
		SDL_DestroyWindow(pWindow);
	}

	WindowState Window::GetState(void) {
		return WndClass.WindowState;
	}

	void Window::PollEvents(void) {
		SDL_Event Event;
		std::vector <SDL_Event> IrrelevantEvents;
		IrrelevantEvents.reserve(2048);
		while (SDL_PollEvent(&Event)) {
			if (Event.window.windowID == SDL_GetWindowID(pWindow)) {
				std::cout << (int)Event.window.event << '\n';
				switch (Event.window.event) {
					case SDL_WINDOWEVENT_CLOSE: {
						WndClass.WindowState = GAMMA_WINDOW_STATE_CLOSED;
					} break; 
					default: {
						EventQueue.push(Event);
					} break;
				}
			} else {
				IrrelevantEvents.push_back(Event);
			}
		}
		for (SDL_Event& IrrelevantEvent : IrrelevantEvents) {
			SDL_PushEvent(&IrrelevantEvent);
		}
	}

}