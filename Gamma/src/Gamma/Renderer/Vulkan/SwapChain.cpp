#include "SwapChain.h"
#include "Instance.h"
#include <SDL_vulkan.h>
#include <iostream>

namespace Gamma {
	namespace Vulkan {

		void SwapChain::CreateSurface(Window* window) {
			// SDL2 provices a cross platform surface creation function
			SDL_Window* InternalWindow = window->GetInternalWindow();
			SDL_Vulkan_CreateSurface(InternalWindow, Instance, &Surface);
			int32_t w, h;
			SDL_Vulkan_GetDrawableSize(InternalWindow, &w, &h);
			DrawableSize.width = w;
			DrawableSize.height = h;
		}

		void SwapChain::Create(VkDevice device, VkSwapchainCreateInfoKHR createinfo) {
			Device = device;
			vkCreateSwapchainKHR(Device, &createinfo, nullptr, &SwapChain);
		}

		void SwapChain::Destroy(void) {
			{
				vkDestroySwapchainKHR(Device, SwapChain, nullptr);
			}
				vkDestroySurfaceKHR(Instance, Surface, nullptr);
			
		}

		VkSurfaceKHR SwapChain::GetSurface(void) {
			return Surface;
		}

		VkExtent2D SwapChain::GetDrawableSize(void) {
			return DrawableSize;
		}

	}
}