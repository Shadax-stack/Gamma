#include "Instance.h"
#include <SDL_vulkan.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <iostream>

namespace Gamma {

	namespace Vulkan {

		VkInstance Instance = nullptr; 
		std::vector<const char*> SupportedRequestedLayers;

		// This function is called when the engine in initialized
		// It is called after SDL_VideoInit()
		void Gamma_Graphics_API_Init(void) {
			const std::vector<const char*> DefaultLayers = {
			#ifdef GAMMA_DEBUG
				"VK_LAYER_KHRONOS_validation",
				"VK_LAYER_LUNARG_standard_validation",
			#endif
			};
			SDL_Vulkan_LoadLibrary(nullptr);
			uint32_t WindowExtCount = 0;
			SDL_Vulkan_GetInstanceExtensions(nullptr, &WindowExtCount, nullptr);
			std::vector<const char*> WindowExts(WindowExtCount);
			SDL_Vulkan_GetInstanceExtensions(nullptr, &WindowExtCount, WindowExts.data());
			uint32_t SupportedLayerCount = 0;
			vkEnumerateInstanceLayerProperties(&SupportedLayerCount, nullptr);
			std::vector<VkLayerProperties> SupportedLayers(SupportedLayerCount);
			vkEnumerateInstanceLayerProperties(&SupportedLayerCount, SupportedLayers.data());
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
			ApplicationInfo.pApplicationName = "Gamma Vulkan Application";
			ApplicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
			VkInstanceCreateInfo InstanceCreateInfo = {};
			InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceCreateInfo.pNext = nullptr;
			InstanceCreateInfo.pApplicationInfo = &ApplicationInfo;
			InstanceCreateInfo.enabledExtensionCount = WindowExts.size();
			InstanceCreateInfo.ppEnabledExtensionNames = WindowExts.data();
			InstanceCreateInfo.enabledLayerCount = SupportedRequestedLayers.size();
			InstanceCreateInfo.ppEnabledLayerNames = SupportedRequestedLayers.data();
			vkCreateInstance(&InstanceCreateInfo, nullptr, &Instance);
		}

		void Gamma_Graphics_API_Quit(void) {
			vkDestroyInstance(Instance, nullptr);
			SDL_Vulkan_UnloadLibrary();
		}

	}

}