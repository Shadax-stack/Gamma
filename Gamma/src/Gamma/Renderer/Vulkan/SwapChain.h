#ifndef GAMMA_VULKAN_SWAP_CHAIN_H
#define GAMMA_VULKAN_SWAP_CHAIN_H

#include "../../Gamma.h"
#include "../../Window/Window.h"
#include <vulkan/vulkan.h>
#include <SDL.h>

namespace Gamma {
	namespace Vulkan {

		class GAMMA_API SwapChain {
		public:
			void CreateSurface(Window* window);
			void Create(VkDevice device, VkSwapchainCreateInfoKHR createinfo);
			void Destroy(void);
			VkSurfaceKHR GetSurface(void);
			VkExtent2D GetDrawableSize(void);
		private:
			VkSurfaceKHR Surface;
			VkSwapchainKHR SwapChain;
			VkDevice Device;
			VkExtent2D DrawableSize;
		};

	}
}

#endif
