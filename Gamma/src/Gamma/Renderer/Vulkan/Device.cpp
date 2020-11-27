#include "Device.h"
#include <vector>
#include <optional>

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
			std::vector<PhysicalDeviceInfo> DevicesInfo(SystemDeviceCount);
			for (const VkPhysicalDevice PhysicalDevice : SystemDevices) {
				DevicesInfo.push_back(PhysicalDevice);
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
			bool IsFullySupported(void) {
				return GraphicsFamily.has_value();
			}
		};

		VkPhysicalDevice PickPhysicalDevice(void) {
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
					if (SupportedQueueFamilies.IsFullySupported()) {
						return PhysicalDeviceInfo.PhysicalDevice;
					}
					QueueFamilyIndex++;
				}
			}
		}

		void Device::Initialize(void) {
			PhysicalDevice = PickPhysicalDevice();
		}

	}
}