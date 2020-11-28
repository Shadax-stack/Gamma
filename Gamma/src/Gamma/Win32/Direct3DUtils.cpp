#include "Direct3DUtils.h"
#include <SDL_syswm.h>
#include <locale>
#include <codecvt>

namespace Gamma {

	namespace Win32 {

		HWND GetWindowHandle(SDL_Window* window) {
			SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(window, &wmInfo);
			return wmInfo.info.win.window;
		}

		HWND GetWindowHandle(Window* window) {
			return GetWindowHandle(window->GetInternalWindow());
		}

		GAMMA_API std::string CreateRegularString(const std::wstring& WideString) {	
			return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(WideString);
		}

	}

}
