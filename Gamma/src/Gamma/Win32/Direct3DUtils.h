#ifndef GAMMA_WIN32_D3DUTILS_H
#define GAMMA_WIN32_D3DUTILS_H

#include "../Gamma.h"
#include "../Window/Window.h"

#include <Windows.h>

#include <string>

namespace Gamma {

	namespace Win32 {

		GAMMA_API HWND GetWindowHandle(SDL_Window* window);
		GAMMA_API HWND GetWindowHandle(Window* window);
		GAMMA_API std::string CreateRegularString(const std::wstring& WideString);

	}

}

#endif // !GAMMA_WIN32_D3DUTILS_H