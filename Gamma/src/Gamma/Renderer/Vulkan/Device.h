#ifndef GAMMA_VULKAN_DEVICE_H
#define GAMMA_VULKAN_DEVICE_H

#include "../../Gamma.h"
#include "Instance.h"
#include <vulkan/vulkan.h>

namespace Gamma {

	namespace Vulkan {

		class GAMMA_API Device {
		public:
			void Initialize(void);
		private:
			VkDevice Device;
			VkPhysicalDevice PhysicalDevice;
		};

	}

}

#endif // !GAMMA_VULKAN_DEVICE_H

