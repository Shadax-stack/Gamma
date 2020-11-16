#include "Window.h"

namespace Gamma {

	WindowClass::WindowClass(void) {}

	void Window::OpenWindow(WindowClass wndclass, const char* title, int32_t width, int32_t height, int32_t xpos, int32_t ypos) {
		WndClass = wndclass;
		pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
	}

	void Window::CloseWindow(void) {
		SDL_DestroyWindow(pWindow);
	}

}