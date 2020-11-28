#include "SwapChain.h"
#include "Instance.h"
#ifdef GAMMA_PLATFORM_WINDOWS
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#include "../../Win32/Direct3DUtils.h"
#endif

namespace Gamma {
	namespace Vulkan {

		// TODO: Add cross-platform surface creation support
		void SwapChain::Create(Window* window) {
#ifdef GAMMA_PLATFORM_WINDOWS
			VkWin32SurfaceCreateInfoKHR SurfaceCreateInfo = {};
			SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			SurfaceCreateInfo.pNext = nullptr;
			SurfaceCreateInfo.hwnd = Win32::GetWindowHandle(window);
			SurfaceCreateInfo.hinstance = GetModuleHandle(NULL);
			vkCreateWin32SurfaceKHR(Instance, &SurfaceCreateInfo, NULL, &Surface);
#endif
		}

		void SwapChain::Destroy(void) {
			vkDestroySurfaceKHR(Instance, Surface, nullptr);
		}

		VkSurfaceKHR SwapChain::GetSurface(void) {
			return Surface;
		}

	}
}