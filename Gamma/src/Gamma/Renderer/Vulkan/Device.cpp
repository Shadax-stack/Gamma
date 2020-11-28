#include "Device.h"
#include <vector>
#include <optional>
#include <iostream>

namespace Gamma {
	namespace Vulkan {

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
					return m_Value;
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

		VkPhysicalDevice PickPhysicalDevice(QueueFamilyIndicies* queuefamilyoutput, VkPhysicalDeviceFeatures* features, SwapChain* swapchain, const char** exts, uint32_t extcount) {
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
							for (uint32_t SupportedExtensionIndex = 0; SupportedExtensionIndex < SupportedExtensionCount; SupportedExtensionCount++) {
								if (!strcmp(RequestedExtension, SupportedExtensions[SupportedExtensionIndex].extensionName)) {
									RequestedSupportedExtensions++;
								}
							}
						}
						if (RequestedSupportedExtensions == extcount) {
							*features = PhysicalDeviceInfo.Features;
							*queuefamilyoutput = SupportedQueueFamilies;
							return PhysicalDeviceInfo.PhysicalDevice;
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
			PhysicalDevice = PickPhysicalDevice(&QueueFamilyIndicies, &PhysicalDeviceFeatures, swapchain, RequestedExtensions, ARRAY_SIZE(RequestedExtensions));
			const int RequestedQueueFamiliesIndicies[] = {
				QueueFamilyIndicies.GraphicsFamily.value(),
				QueueFamilyIndicies.PresentFamily.value(),
			};
			// Why would you store the requested queue families in a vector?
			// The number of queues should be known at runtime 
			// If it's not then it's either:
			//  1. An application with some sort of special requirement or need
			//  2. Overengineered code that trades performance for supporting 99% of scenarios 
			VkDeviceQueueCreateInfo RequestedQueueFamiliesCreateInfo[ARRAY_SIZE(RequestedQueueFamiliesIndicies)];
			const float DefaultQueuePriority = 1.0f;
			for (uint32_t Index = 0; Index < ARRAY_SIZE(RequestedQueueFamiliesIndicies); Index++) {
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
			DeviceCreateInfo.pQueueCreateInfos = RequestedQueueFamiliesCreateInfo;
			DeviceCreateInfo.queueCreateInfoCount = ARRAY_SIZE(RequestedQueueFamiliesIndicies);
			DeviceCreateInfo.ppEnabledLayerNames = SupportedRequestedLayers.data();
			DeviceCreateInfo.enabledLayerCount = SupportedRequestedLayers.size();
			DeviceCreateInfo.ppEnabledExtensionNames = RequestedExtensions;
			DeviceCreateInfo.enabledExtensionCount = ARRAY_SIZE(RequestedExtensions);
			vkCreateDevice(PhysicalDevice, &DeviceCreateInfo, nullptr, &Device);
			// Eventually we will output these into parameters, for example, a renderer should take care of the queues, not the device
			vkGetDeviceQueue(Device, QueueFamilyIndicies.GraphicsFamily.value(), 0, &GraphicsQueue);
			vkGetDeviceQueue(Device, QueueFamilyIndicies.PresentFamily.value(), 0, &PresentQueue);
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