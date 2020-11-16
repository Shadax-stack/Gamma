#include "../Gamma.h"
#include <SDL.h>
#include <stdint.h>
#include <queue>

namespace Gamma {

	enum Fullscreen {
		WINDOWED = 0,
		WINDOWED_FULLSCREEN = SDL_WINDOW_FULLSCREEN,
		FULLSCREEN = SDL_WINDOW_FULLSCREEN_DESKTOP
	};

	struct GAMMA_API WindowClass {
	public:
		//Initialize the window class members to a certain default value
		WindowClass(void);
		//Basic concped is based off the WINAPI window class
		Fullscreen FullscreenState;

	};

	class GAMMA_API Window {
	public:
		void OpenWindow(WindowClass wndclass, const char* title, int32_t width, int32_t height, int32_t xpos = 0, int32_t ypos = 0);
		void CloseWindow(void);
	private:
		WindowClass WndClass;
		SDL_Window* pWindow;
		friend class Renderer;
	};

}