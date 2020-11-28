#ifndef GAMMA_VULKAN_DEVICE_H
#define GAMMA_VULKAN_DEVICE_H

#include "../../Gamma.h"
#include "Instance.h"
#include "SwapChain.h"
#include <vulkan/vulkan.h>

namespace Gamma {

	namespace Vulkan {

		class GAMMA_API Device {
		public:
			void Create(SwapChain* swapchain);
			void Destroy(void);
			operator VkPhysicalDeviceProperties(void);
			operator VkPhysicalDeviceFeatures(void);
		private:
			VkDevice Device;
			VkPhysicalDevice PhysicalDevice;
			VkQueue GraphicsQueue;
			VkQueue PresentQueue;
		};

	}

}

#endif // !GAMMA_VULKAN_DEVICE_H

