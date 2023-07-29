#include <Phoenix/vk/PhysDevice.hpp>
#include <Phoenix/vk/Surface.hpp>

namespace phoenix {
	namespace vk {

		


		std::vector<VkPhysicalDevice> GetPhysicalDevices(Context* ctx) {
			uint32_t device_count = 0;
			vkEnumeratePhysicalDevices(ctx->GetInstance(), &device_count, nullptr);
			if (device_count == 0) {
				std::cerr << "[Pheonix] - No Devices Support Vulkan\n";
				exit(EXIT_FAILURE);
			}

			std::vector<VkPhysicalDevice> devices(device_count);
			vkEnumeratePhysicalDevices(ctx->GetInstance(), &device_count, devices.data());

			return devices;
		}

		PhysDevice::PhysDevice(VkPhysicalDevice device, Context* ctx) : m_device(device), m_ctx(ctx), m_surface(ctx->GetVKSurface()) {
			
		}

		VkPhysicalDeviceProperties PhysDevice::Properties()
		{
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(m_device, &props);
			return props;
		}

		std::string PhysDevice::Name() {
			return std::string(Properties().deviceName);
		}

		
		uint32_t PhysDevice::ID()
		{
			return Properties().deviceID;
		}


		bool PhysDevice::IsSuitable()
		{
			bool extensionCheckPassed = CheckExtensionSupport();

			bool swapChainAdequate = false;
			if (extensionCheckPassed) {
				SwapChainSupportDetails swapChainSupport = GetSwapChainSupport();
				swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}

			return GetIndices().isComplete() && extensionCheckPassed && swapChainAdequate;
		}

		SwapChainSupportDetails PhysDevice::GetSwapChainSupport()
		{
			SwapChainSupportDetails details;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device, m_surface->Get(), &details.capabilities);

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, m_surface->Get(), &formatCount, nullptr);

			if (formatCount != 0) {
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, m_surface->Get(), &formatCount, details.formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, m_surface->Get(), &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, m_surface->Get(), &presentModeCount, details.presentModes.data());
			}

			return details;
		}

		VkExtent2D PhysDevice::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
				return capabilities.currentExtent;
			}
			else {
				int width, height;
				glfwGetFramebufferSize(m_ctx->GetWindow(), &width, &height);

				VkExtent2D actualExtent = {
					static_cast<uint32_t>(width),
					static_cast<uint32_t>(height)
				};

				actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return actualExtent;
			}
		}


		std::vector<VkQueueFamilyProperties> PhysDevice::GetQueueFamilyProps() {
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(m_device, &queueFamilyCount, queueFamilies.data());
			return queueFamilies;
		}

		bool PhysDevice::CheckExtensionSupport()
		{
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(m_device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

			for (const auto& extension : availableExtensions) {
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		QueueFamilyIndices PhysDevice::GetIndices() {
			if (m_indices.has_value()) { return m_indices.value(); }

			m_indices = FindQueueFamilies();
			return m_indices.value();
		}

		QueueFamilyIndices PhysDevice::FindQueueFamilies() {
			QueueFamilyIndices indices{};
			int i = 0;
			std::vector<VkQueueFamilyProperties> queueFamilies = GetQueueFamilyProps();
			for (const auto& queueFamily : queueFamilies) {
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_device, i, m_surface->Get(), &presentSupport);

				if (presentSupport) {
					indices.presentFamily = i;
				}

				if (indices.isComplete()) {
					break;
				}

				i++;
			}
			return indices;
		}



		std::shared_ptr<PhysDevice> GetPhysicalDevice(Context* ctx) {
			return std::make_shared<PhysDevice>(GetPhysicalDevices(ctx)[0], ctx);
		}
	}
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == vk::prefs::FORMAT && availableFormat.colorSpace == vk::prefs::COLOR_SPACE) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == vk::prefs::PRESENT_MODE) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}
}