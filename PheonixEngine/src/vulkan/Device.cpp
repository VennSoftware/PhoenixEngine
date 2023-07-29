#include <Phoenix/vk/Device.hpp>
#include <Phoenix/vk/Surface.hpp>
#include <set>

namespace phoenix {
	namespace vk {
		Device::Device(std::shared_ptr<PhysDevice> physDevice, Context* ctx) : m_physDevice(physDevice), m_ctx(ctx) {
			QueueFamilyIndices indices = m_physDevice->GetIndices();
			VkDeviceQueueCreateInfo queue_info{};


			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

			float queuePriority = 1.0f;
			for (uint32_t queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.queueCreateInfoCount = queueCreateInfos.size();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = (uint32_t) deviceExtensions.size();
			createInfo.ppEnabledExtensionNames = deviceExtensions.data();

			if (enableValidationLayers) {
				createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
				createInfo.ppEnabledLayerNames = validationLayers.data();
			}
			else {
				createInfo.enabledLayerCount = 0;
			}

			if (vkCreateDevice(m_physDevice->Get(), &createInfo, nullptr, &m_device) != VK_SUCCESS) {
				std::cerr << "Failed To Create Logical Device\n";
				exit(EXIT_FAILURE);
			}

			vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
			vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);

			CreateSwapChain();
		}
		void Device::Destroy()
		{
			for (auto imageView : m_swapchainImageViews) {
				vkDestroyImageView(m_device, imageView, nullptr);
			}
			vkDestroyDevice(m_device, nullptr);
		}

		void Device::CreateSwapChain() {
			SwapChainSupportDetails swapChainSupport = m_physDevice->GetSwapChainSupport();

			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = m_physDevice->chooseSwapExtent(swapChainSupport.capabilities);

			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = m_ctx->GetVKSurface()->Get();

			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


			QueueFamilyIndices indices = m_physDevice->FindQueueFamilies();
			uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

			if (indices.graphicsFamily != indices.presentFamily) {
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0; // Optional
				createInfo.pQueueFamilyIndices = nullptr; // Optional
			}
			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.oldSwapchain = VK_NULL_HANDLE;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			VkSwapchainKHR swapChain;

			if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
				throw std::runtime_error("failed to create swap chain!");
			}

			std::cout << "[Phoenix] - Created Swapchain\n";

			m_format = surfaceFormat.format;
			m_extent = extent;

			vkGetSwapchainImagesKHR(m_device, swapChain, &imageCount, nullptr);
			m_swapchainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(m_device, swapChain, &imageCount, m_swapchainImages.data());

			m_swapchainImageViews.resize(m_swapchainImages.size());
			for (size_t i = 0; i < m_swapchainImages.size(); i++) {
				VkImageViewCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = m_swapchainImages[i];

				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = m_format;

				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				if (vkCreateImageView(m_device, &createInfo, nullptr, &m_swapchainImageViews[i]) != VK_SUCCESS) {
					std::cout << "[Phoenix] - Failed To Create Image View\n";
					exit(EXIT_FAILURE);
				}
			}
			m_swapchain = swapChain;
			std::cout << "[Phoenix] - Created " << m_swapchainImageViews.size() << " Images\n";
		}
		void Device::InvalidateSwapChain()
		{

			int width = 0, height = 0;
			glfwGetFramebufferSize(m_ctx->m_window, &width, &height);
			while (width == 0 || height == 0) {
				glfwGetFramebufferSize(m_ctx->m_window, &width, &height);
				glfwWaitEvents();
			}

			WaitForIdle();

			PHNX_LOG("Invalidating Swapchain");

			for (size_t i = 0; i < m_ctx->GetFrameBuffers().size(); i++) {
				vkDestroyFramebuffer(m_device, m_ctx->GetFrameBuffers()[i], nullptr);
			}

			for (size_t i = 0; i < m_swapchainImageViews.size(); i++) {
				vkDestroyImageView(m_device, m_swapchainImageViews[i], nullptr);
			}

			vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

			CreateSwapChain();
			
			m_ctx->CreateFrameBuffers();
		}
	}
}
