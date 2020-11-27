#include "Instance.h"
#include <SDL_vulkan.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>

namespace Gamma {

	namespace Vulkan {

		VkInstance Instance = nullptr; 

		void Gamma_Graphics_API_Init(void) {
			// TODO: Find a better way to do this without using 10000 vectors and allocations 
			const std::vector<const char*> DefaultLayers = {
			#ifdef GAMMA_DEBUG
				"VK_LAYER_KHRONOS_validation",
				"VK_LAYER_LUNARG_standard_validation",
			#endif
			};
			uint32_t WindowExtensionCount = 0;
			SDL_Vulkan_GetInstanceExtensions(nullptr, &WindowExtensionCount, nullptr);
			std::vector<const char*> WindowExtensions(WindowExtensionCount);
			SDL_Vulkan_GetInstanceExtensions(nullptr, nullptr, WindowExtensions.data());
			uint32_t SupportedLayerCount = 0;
			vkEnumerateInstanceLayerProperties(&SupportedLayerCount, nullptr);
			std::vector<VkLayerProperties> SupportedLayers(SupportedLayerCount);
			vkEnumerateInstanceLayerProperties(&SupportedLayerCount, SupportedLayers.data());
			std::vector<const char*> SupportedRequestedLayers;
			for (const char* RequestedLayer : DefaultLayers) {
				for (VkLayerProperties SupportedLayer : SupportedLayers) {
					if (!strcmp(RequestedLayer, SupportedLayer.layerName)) {
						SupportedRequestedLayers.push_back(RequestedLayer);
						break;
					}
				}
			}
			VkApplicationInfo ApplicationInfo = {};
			ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			ApplicationInfo.pNext = nullptr;
			ApplicationInfo.apiVersion = VK_API_VERSION_1_0;
			ApplicationInfo.pEngineName = "Gamma Engine";
			ApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			ApplicationInfo.pApplicationName = nullptr;
			ApplicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
			VkInstanceCreateInfo InstanceCreateInfo = {};
			InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceCreateInfo.pNext = nullptr;
			InstanceCreateInfo.pApplicationInfo = &ApplicationInfo;
			InstanceCreateInfo.enabledLayerCount = 0;
			InstanceCreateInfo.enabledExtensionCount = WindowExtensions.size();
			InstanceCreateInfo.ppEnabledLayerNames = WindowExtensions.data();
			InstanceCreateInfo.enabledLayerCount = SupportedRequestedLayers.size();
			InstanceCreateInfo.ppEnabledLayerNames = SupportedRequestedLayers.data();
			vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance);
		}

		void Gamma_Graphics_API_Quit(void) {
			vkDestroyInstance(Instance, nullptr);
		}

	}

}