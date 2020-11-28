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
			void Create(Window* window);
			void Destroy(void);
			VkSurfaceKHR GetSurface(void);
		private:
			VkSurfaceKHR Surface;
		};

	}
}

#endif
