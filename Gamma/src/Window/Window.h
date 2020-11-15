#include <stdint.h>
#include <SDL.h>

namespace Gamma {

	struct WindowClass {
	public:
		//Initialize the window class members to a certain default value
		WindowClass(void);
		//Basic concped is based off the WINAPI window class
	};

	class Window {
	public:
		void OpenWindow(WindowClass* wndclass, const char* title, uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos);
	private:
		SDL_Window* pWindow;
	};

}