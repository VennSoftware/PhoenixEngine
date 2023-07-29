#pragma once
#include "Context.hpp"
#include "PhysDevice.hpp"

namespace phoenix {
	namespace vk {
		class Device {
		public:
			Device(std::shared_ptr<PhysDevice> physDevice, Context* ctx);


			void Destroy();

			void CreateSwapChain();
			void InvalidateSwapChain();


			VkQueue GetGraphicsQueue();
			std::vector<VkImage>& GetImages() { return m_swapchainImages; }
			std::vector<VkImageView>& GetImageViews() { return m_swapchainImageViews; }
			VkFormat GetFormat() { return m_format; }
			VkExtent2D GetExtent() { return m_extent; }
			VkDevice Get() { return m_device; }
			Context* GetContext() { return m_ctx; }

			VkSwapchainKHR GetSwapchain() { return m_swapchain; }

			VkQueue GetPresentQueue();

			void WaitForIdle() { vkDeviceWaitIdle(m_device); }



			

		private:
			std::shared_ptr<PhysDevice> m_physDevice;
			VkDevice m_device = VK_NULL_HANDLE;
			VkQueue m_graphicsQueue = VK_NULL_HANDLE;
			VkQueue m_presentQueue = VK_NULL_HANDLE;
			VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
			VkFormat m_format;
			VkExtent2D m_extent;
			Context* m_ctx;
			std::vector<VkImage> m_swapchainImages;
			std::vector<VkImageView> m_swapchainImageViews;

			friend RenderQueue;
			friend Context;
		};
	}
}