#pragma once
#include "Context.hpp"
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp

namespace phoenix {
	namespace vk {
		class PhysDevice {
		public:
			PhysDevice(VkPhysicalDevice device, Context* ctx);

			
			VkPhysicalDeviceProperties Properties();
			std::string Name();
			uint32_t ID();
			QueueFamilyIndices FindQueueFamilies();
			QueueFamilyIndices GetIndices();

			operator VkPhysicalDevice () {
				return m_device;
			}

			bool IsSuitable();

			VkPhysicalDevice Get() { return m_device; }

			SwapChainSupportDetails GetSwapChainSupport();
		

			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			
			
		protected:
			std::vector<VkQueueFamilyProperties> GetQueueFamilyProps();
			bool CheckExtensionSupport();
			
		private:
			VkPhysicalDevice m_device;
			Context* m_ctx;
			std::optional<QueueFamilyIndices> m_indices;
			std::shared_ptr<Surface> m_surface;
		};


		std::vector<VkPhysicalDevice> GetPhysicalDevices(Context* ctx);

		std::shared_ptr<PhysDevice> GetPhysicalDevice(Context* ctx);
	}


	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);



}