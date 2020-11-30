#include "Device.h"
#include <vector>
#include <iostream>
#include <algorithm>
#ifdef GAMMA_CXX17
#include <optional>
#endif

namespace Gamma {
	namespace Vulkan {

		VkPresentModeKHR DefaultPreferredPresentModes[] = {
			VK_PRESENT_MODE_FIFO_RELAXED_KHR, // "Best" present mode
			VK_PRESENT_MODE_MAILBOX_KHR,      // Some latency is ok
			VK_PRESENT_MODE_IMMEDIATE_KHR,    // Immediate presentation is better than stuterring
			VK_PRESENT_MODE_FIFO_KHR,         // At the bottom where it belongs
		};

		struct PhysicalDeviceInfo {
			VkPhysicalDevice PhysicalDevice;
			VkPhysicalDeviceProperties Properties;
			VkPhysicalDeviceFeatures Features;
			PhysicalDeviceInfo(VkPhysicalDevice physicalDevice) : PhysicalDevice(physicalDevice) {
				vkGetPhysicalDeviceProperties(PhysicalDevice, &Properties);
				vkGetPhysicalDeviceFeatures(PhysicalDevice, &Features);
			}
			PhysicalDeviceInfo(void) = default;
		};

		std::vector<PhysicalDeviceInfo> GetSystemPhysicalDevicesInfo(void) {
			uint32_t SystemDeviceCount = 0;
			vkEnumeratePhysicalDevices(Instance, &SystemDeviceCount, nullptr);
			std::vector<VkPhysicalDevice> SystemDevices(SystemDeviceCount);
			vkEnumeratePhysicalDevices(Instance, &SystemDeviceCount, SystemDevices.data());
			std::vector<PhysicalDeviceInfo> DevicesInfo;
			DevicesInfo.reserve(SystemDeviceCount);
			for (const VkPhysicalDevice PhysicalDevice : SystemDevices) {
				DevicesInfo.emplace_back(PhysicalDevice);
			}
			return DevicesInfo;
		}

		struct QueueFamilyIndicies {
			#ifdef GAMMA_CXX17
			typedef std::optional<uint32_t> QueueFamilyIndex;
			#else
			struct QueueFamilyIndex {
				QueueFamilyIndex() : m_IsSet(false) {

				}
				operator uint32_t& () {
					return m_Value;
				}
				void operator=(uint32_t val) {
					m_Value = val;
					m_IsSet = true;
				}
				bool has_value() {
					return m_IsSet;
				}
				uint32_t& value() {
					if (m_IsSet) {
						return m_Value;
					}
					else {
						// or some other way to throw an exception
						abort();
						// Return 0
						return 0;
					}
				}
			private:
				uint32_t m_Value;
				bool m_IsSet;
			};
			#endif
			QueueFamilyIndex GraphicsFamily;
			QueueFamilyIndex PresentFamily;
			bool IsFullySupported(void) {
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}
		};

		struct SwapChainSupportInfo {
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
			void CreateInfo(VkPhysicalDevice PhysicalDevice, SwapChain* swapchain) {
				VkSurfaceKHR Surface = swapchain->GetSurface();
				vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevice, Surface, &Capabilities);
				struct {
					uint32_t Formats;
					uint32_t PresentModes;
				} Count;
				vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &Count.Formats, nullptr);
				if (Count.Formats) {
					Formats.resize(Count.Formats);
					vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevice, Surface, &Count.Formats, Formats.data());
				}
				vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &Count.PresentModes, nullptr);
				if (Count.PresentModes) {
					PresentModes.resize(Count.PresentModes);
					vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevice, Surface, &Count.PresentModes, PresentModes.data());
				}
			}
			bool IsUseable(void) {
				return !(Formats.empty() || PresentModes.empty());
			}
			bool Has(VkFormat format, VkColorSpaceKHR colorspace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR, uint32_t* index = nullptr) {
				VkSurfaceFormatKHR Format;
				Format.format = format;
				Format.colorSpace = colorspace;
				struct {
					uint32_t index = 0;
					bool found = false;
					operator uint32_t& (void) {
						return index;
					}
					void operator++(void) {
						index++;
					}
				} SearchResult;
				for (VkSurfaceFormatKHR FormatItr : Formats) {
					if (FormatItr.colorSpace == Format.colorSpace || FormatItr.format == Format.format) {
						SearchResult.found = true;
						break;
					}
					else {
						SearchResult++;
					}
				}
				if (SearchResult.found) {
					if (index) {
						*index = SearchResult.index;
					}
					return true;
				}
				else {
					// The else isn't really nessacy but whatever
					*index = 0;
					return false;
				}
			}

			VkSurfaceFormatKHR GetUseableFormat(VkFormat format, VkColorSpaceKHR colorspace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				uint32_t index;
				Has(format, colorspace, &index);
				return Formats[index];
			}

			VkPresentModeKHR GetPresentMode(VkPresentModeKHR* modes = DefaultPreferredPresentModes, uint32_t size = ARRAY_SIZE(DefaultPreferredPresentModes)) {
				for (uint32_t index = 0; index < size; index++) {
					VkPresentModeKHR PreferredPresentMode = modes[index];
					for (VkPresentModeKHR PresentMode : PresentModes) {
						if (PreferredPresentMode == PresentMode) {
							return PreferredPresentMode;
						}
					}
				}
				return VK_PRESENT_MODE_FIFO_KHR;
			}
		};

		VkPhysicalDevice PickPhysicalDevice(QueueFamilyIndicies* queuefamilyoutput, VkPhysicalDeviceFeatures* features, SwapChain* swapchain, const char** exts, uint32_t extcount, SwapChainSupportInfo* swapchainsupport) {
			std::vector<PhysicalDeviceInfo> SystemPhysicalDevicesInfo = GetSystemPhysicalDevicesInfo();
			for (const PhysicalDeviceInfo PhysicalDeviceInfo : SystemPhysicalDevicesInfo) {
				// TODO: Add a proper system to select the GPU based of "device score"
				uint32_t QueueFamilyCount = 0;
				vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDeviceInfo.PhysicalDevice, &QueueFamilyCount, nullptr);
				std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDeviceInfo.PhysicalDevice, &QueueFamilyCount, QueueFamilies.data());
				QueueFamilyIndicies SupportedQueueFamilies;
				uint32_t QueueFamilyIndex = 0;
				for (VkQueueFamilyProperties QueueFamilyProperties : QueueFamilies) {
					if (QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						SupportedQueueFamilies.GraphicsFamily = QueueFamilyIndex;
					}
					VkBool32 PresentSupport = false;
					vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDeviceInfo.PhysicalDevice, QueueFamilyIndex, swapchain->GetSurface(), &PresentSupport);
					if (PresentSupport) {
						SupportedQueueFamilies.PresentFamily = QueueFamilyIndex;
					}
					if (SupportedQueueFamilies.IsFullySupported()) {
						// Check whether it supports the extensions we want
						uint32_t SupportedExtensionCount = 0;
						vkEnumerateDeviceExtensionProperties(PhysicalDeviceInfo.PhysicalDevice, nullptr, &SupportedExtensionCount, nullptr);
						VkExtensionProperties* SupportedExtensions = new VkExtensionProperties[SupportedExtensionCount];
						vkEnumerateDeviceExtensionProperties(PhysicalDeviceInfo.PhysicalDevice, nullptr, &SupportedExtensionCount, SupportedExtensions);
						uint32_t RequestedSupportedExtensions = 0;
						// Loop through requested extensions instead of all extensions first because usually only a few exts will be requested instead of all of them
						for (uint32_t RequestedExtensionIndex = 0; RequestedExtensionIndex < extcount; RequestedExtensionIndex++) {
							// Calculate the extension name address only once
							const char* RequestedExtension = exts[RequestedExtensionIndex];
							for (uint32_t SupportedExtensionIndex = 0; SupportedExtensionIndex < SupportedExtensionCount; SupportedExtensionIndex++) {
								if (!strcmp(RequestedExtension, SupportedExtensions[SupportedExtensionIndex].extensionName)) {
									RequestedSupportedExtensions++;
								}
							}
						}
						if (RequestedSupportedExtensions == extcount) {
							// Get swap chain info
							swapchainsupport->CreateInfo(PhysicalDeviceInfo.PhysicalDevice, swapchain);
							if (swapchainsupport->IsUseable()) {
								*features = PhysicalDeviceInfo.Features;
								*queuefamilyoutput = SupportedQueueFamilies;
								return PhysicalDeviceInfo.PhysicalDevice;
								
							}
						}
					}
					QueueFamilyIndex++;
				}
			}
			return nullptr;
		}

		void Device::Create(SwapChain* swapchain) { 
			const char* RequestedExtensions[] = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			};
			QueueFamilyIndicies QueueFamilyIndicies;
			VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
			SwapChainSupportInfo SwapChainInfo;
			PhysicalDevice = PickPhysicalDevice(&QueueFamilyIndicies, &PhysicalDeviceFeatures, swapchain, RequestedExtensions, ARRAY_SIZE(RequestedExtensions), &SwapChainInfo);
			std::vector<uint32_t> RequestedQueueFamiliesIndicies = {
				QueueFamilyIndicies.GraphicsFamily.value(),
				QueueFamilyIndicies.PresentFamily.value(),
			};
			// Make the queues unique so we avoid having 2 queues with the same index
			// This allows us to support both graphics cards that make present and graphics queue families the same index and those that don't without hardcoding support for either 
			RequestedQueueFamiliesIndicies.resize(std::distance(RequestedQueueFamiliesIndicies.begin(), std::unique(RequestedQueueFamiliesIndicies.begin(), RequestedQueueFamiliesIndicies.end())));
			std::vector<VkDeviceQueueCreateInfo> RequestedQueueFamiliesCreateInfo(RequestedQueueFamiliesIndicies.size());
			const float DefaultQueuePriority = 1.0f;
			for (uint32_t Index = 0; Index < RequestedQueueFamiliesIndicies.size(); Index++) {
				VkDeviceQueueCreateInfo DeviceQueueCreateInfo = {};
				DeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				DeviceQueueCreateInfo.pNext = nullptr;
				DeviceQueueCreateInfo.queueFamilyIndex = RequestedQueueFamiliesIndicies[Index];
				DeviceQueueCreateInfo.queueCount = 1;
				DeviceQueueCreateInfo.pQueuePriorities = &DefaultQueuePriority;
				// Because if I did write directly to the array the address would have to be calculated each time I wanted to write a value
				RequestedQueueFamiliesCreateInfo[Index] = DeviceQueueCreateInfo;
			}
			VkDeviceCreateInfo DeviceCreateInfo = {};
			DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			DeviceCreateInfo.pNext = nullptr;
			DeviceCreateInfo.pEnabledFeatures = &PhysicalDeviceFeatures;
			DeviceCreateInfo.pQueueCreateInfos = RequestedQueueFamiliesCreateInfo.data();
			DeviceCreateInfo.queueCreateInfoCount = RequestedQueueFamiliesCreateInfo.size();
			DeviceCreateInfo.ppEnabledLayerNames = SupportedRequestedLayers.data();
			DeviceCreateInfo.enabledLayerCount = SupportedRequestedLayers.size();
			DeviceCreateInfo.ppEnabledExtensionNames = RequestedExtensions;
			DeviceCreateInfo.enabledExtensionCount = ARRAY_SIZE(RequestedExtensions);
			vkCreateDevice(PhysicalDevice, &DeviceCreateInfo, nullptr, &Device);
			// Eventually we will output these into parameters, for example, a renderer should take care of the queues, not the device
			// Basiclly the device will be for initialization and pretty low level stuff
			// However this might get in the way of the planned abstraction model
			// The abstraction model is split into "layers". Here they are from top to bottom
			// 1. Application layer - deals with game logic and telling the renderer to render meshs and stuff
			// 2. Engine high level - deals with basic rendering stuff. Anything at this layer should be fully cross platform and not dependent on rendering api
			//    If it is dependent, then the issue should be moved to the low level layer, although a ifdef should solve the issue. Cross platform libraries are used on
			//    this layer.
			// 3. Engine low level - Things that are not portable like rendering API and other things like threads should be done on this layer. For each thing that isn't
			//    portable, like rendering APi, it should be split up into different files. Cross platform and platform dependent libraries are on this level
			// Here is an example: Let's say the user wants to load a texture with a given path. The engine high level layer exposes a function called "LoadTexture2D" which
			// returns a 2D texture object. It will take in a string path. The implementation details may be:
			// 1. Call a function from an image loading library to load the image into an unsigned char* array
			// 2. Call a function to create a 2D texture object. This function is provided by the low level layer
			// 3. Load the data into the texture using another function, which is again from the low level layer
			// 4. Return the object
			// The low level layer functions interact directly with rendering APIs such as Vulkan and Direct3D 12 to manipulate the texture object. 
			// The way this current set up conflicts with this is that the renderer should be on layer 2, and queues are rendering API dependent 
			// I will find a solution to this later
			vkGetDeviceQueue(Device, QueueFamilyIndicies.GraphicsFamily.value(), 0, &GraphicsQueue);
			vkGetDeviceQueue(Device, QueueFamilyIndicies.PresentFamily.value(), 0, &PresentQueue);
			// Now create a swap chain
			// I will not do any useless checking for swap chain support of capabalites since I plan drop support for ancient GPUs
			// Because the engine is geared towards quality and performance, not low quality and ok performance
			// A real engine would check for support but I won't, you need to get a newer GPU if you cannot run the engine
			VkSwapchainCreateInfoKHR SwapChainCreateInfo = {};
			SwapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			SwapChainCreateInfo.pNext = nullptr;
			SwapChainCreateInfo.surface = swapchain->GetSurface();
			SwapChainCreateInfo.minImageCount = 2; // Double buffering
			SwapChainCreateInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;
			SwapChainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			SwapChainCreateInfo.imageExtent = swapchain->GetDrawableSize();
			SwapChainCreateInfo.imageArrayLayers = 1;
			SwapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			uint32_t GraphicsPresentFamilies[] = {
				QueueFamilyIndicies.GraphicsFamily.value(),
				QueueFamilyIndicies.PresentFamily.value(),
			};
			if (QueueFamilyIndicies.GraphicsFamily != QueueFamilyIndicies.PresentFamily) {
				SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				SwapChainCreateInfo.queueFamilyIndexCount = 2;
				SwapChainCreateInfo.pQueueFamilyIndices = GraphicsPresentFamilies;
			}
			else {
				SwapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				SwapChainCreateInfo.queueFamilyIndexCount = 0;
				SwapChainCreateInfo.pQueueFamilyIndices = nullptr; 
			}
			SwapChainCreateInfo.queueFamilyIndexCount = RequestedQueueFamiliesIndicies.size();
			SwapChainCreateInfo.pQueueFamilyIndices = RequestedQueueFamiliesIndicies.data();
			SwapChainCreateInfo.preTransform = SwapChainInfo.Capabilities.currentTransform;
			SwapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			SwapChainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
			SwapChainCreateInfo.clipped = VK_TRUE;
			SwapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
			swapchain->Create(Device, SwapChainCreateInfo);
		}

		void Device::Destroy(void) {
			vkDestroyDevice(Device, nullptr);
		}

		Device::operator VkPhysicalDeviceProperties(void) {
			VkPhysicalDeviceProperties PhysicalDeviceProperties;
			vkGetPhysicalDeviceProperties(PhysicalDevice, &PhysicalDeviceProperties);
			return PhysicalDeviceProperties;
		}

		Device::operator VkPhysicalDeviceFeatures(void) {
			VkPhysicalDeviceFeatures PhysicalDeviceFeatures;
			vkGetPhysicalDeviceFeatures(PhysicalDevice, &PhysicalDeviceFeatures);
			return PhysicalDeviceFeatures;
		}

	}
}