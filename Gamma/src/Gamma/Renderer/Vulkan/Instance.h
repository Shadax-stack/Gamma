#ifndef GAMMA_VULKAN_INSTANCE_H
#define GAMMA_VULKAN_INSTANCE_H

#include "../../Gamma.h"
#include <vulkan/vulkan.h>
#include <vector>

namespace Gamma {

	namespace Vulkan {

		// The global instance that Gamma engine should use
		extern VkInstance Instance;
		extern std::vector<const char*> SupportedRequestedLayers;

	}

}

#endif