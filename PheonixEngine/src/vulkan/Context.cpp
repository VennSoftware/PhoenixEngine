#include <Phoenix/vk/Context.hpp>
#include <Phoenix/vk/PhysDevice.hpp>
#include <Phoenix/vk/Device.hpp>
#include <Phoenix/vk/Surface.hpp>
#include <Phoenix/vk/GraphicsPipeline.hpp>
#include <Phoenix/vk/RenderQueue.hpp>
namespace phoenix {
	namespace vk {


		static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
			auto app = reinterpret_cast<Context*>(glfwGetWindowUserPointer(window));
			app->framebufferResized = true;
		}


		Context::Context(const ContextSpec& spec) : m_spec(spec) {
			InitializeWindow();
			InitializeVulkan();
		}

		void Context::InitializeWindow() {
			m_height = m_spec.height;
			m_width = m_spec.width;
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
			m_window = glfwCreateWindow(m_width, m_height, m_spec.title, NULL, NULL);
			glfwSetWindowUserPointer(m_window, this);
			glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
			if (!m_window) {
				PHNX_FATAL("Failed To Create Window");
			}

			std::cout << "[Pheonix] - Created Window (" << m_spec.width << "x" << m_spec.height << ")\n";
		}

		void Context::InitializeVulkan() {
			
			CreateInstance();

			m_surface = std::make_shared<Surface>(this);

			m_physDevice = GetPhysicalDevice(this);
			std::cout << m_physDevice->Name() << " - Suitable?: " << std::boolalpha << m_physDevice->IsSuitable() << "\n";
			if (!m_physDevice->IsSuitable()) {
				exit(-1);
			}

			m_device = std::make_shared<Device>(m_physDevice, this);


			m_pipeline = std::make_shared<PipelineBuilder>(m_device);


			CreateFrameBuffers();


			m_renderQueue = std::make_shared<RenderQueue>(this);

			
			CreateSyncObjects();

			glfwShowWindow(m_window);
		}

		void Context::Destroy() {
			
			std::cout << "[Pheonix] - Destroying...\n";
			for (auto framebuffer : swapChainFramebuffers) {
				vkDestroyFramebuffer(m_device->Get(), framebuffer, nullptr);
			}
			m_device->Destroy();
			m_surface->Destroy();
			vkDestroyInstance(instance, nullptr);
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		bool Context::IsActive() {
			return !glfwWindowShouldClose(m_window);
		}

		void Context::PollEvents() {
			glfwPollEvents();
		}

		void Context::StartFrame()
		{
			vkWaitForFences(m_device->Get(), 1, &inFlightFence, VK_TRUE, UINT64_MAX);
			VkResult result = vkAcquireNextImageKHR(m_device->Get(), m_device->GetSwapchain(), UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
				m_device->InvalidateSwapChain();
				framebufferResized = false;
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
				throw std::runtime_error("failed to acquire swap chain image!");
			}
			m_renderQueue->BeginPass(imageIndex, ColorRGBA{ 0, 0, 0, 1 });
		}

		void Context::EndFrame()
		{
			m_renderQueue->SubmitPass();

			VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };

			m_renderQueue->SubmitCmdBuffer(waitSemaphores, waitStages, signalSemaphores);

			VkPresentInfoKHR presentInfo{};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { m_device->GetSwapchain()};
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &imageIndex;

			VkResult result = vkQueuePresentKHR(m_device->m_presentQueue, &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
				m_device->InvalidateSwapChain();
				framebufferResized = false;
			}
			else if (result != VK_SUCCESS) {
				throw std::runtime_error("failed to present swap chain image!");
			}
		}

		std::vector<VkLayerProperties> Context::LayerProps()
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			return availableLayers;
		}


		void Context::CreateInstance() {
			if (enableValidationLayers && !CheckValidationLayerSupport()) {
				PHNX_FATAL("Requested Validation, No Layers Found.");
			}


			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = m_spec.appName;
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = m_spec.engineName;
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;

			createInfo.enabledLayerCount = 0;

			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
				PHNX_FATAL("Failed To Create Vulkan Instance");
			}
		}

		bool Context::CheckValidationLayerSupport() {
			std::vector<VkLayerProperties> availableLayers = LayerProps();

			for (const char* layerName : validationLayers) {
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		void Context::CreateFrameBuffers()
		{
			
			swapChainFramebuffers.resize(m_device->GetImageViews().size());
			for (size_t i = 0; i < m_device->GetImageViews().size(); i++) {
				VkImageView attachments[] = {
					m_device->GetImageViews()[i]
				};

				VkFramebufferCreateInfo framebufferInfo{};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = m_pipeline->GetRenderPass();
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = m_device->GetExtent().width;
				framebufferInfo.height = m_device->GetExtent().height;
				framebufferInfo.layers = 1;

				if (vkCreateFramebuffer(m_device->Get(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
					PHNX_FATAL("failed to create framebuffer!");
				}
			}

			PHNX_LOG("Created %u Framebuffers", swapChainFramebuffers.size());
		}
		void Context::CreateSyncObjects()
		{

			VkSemaphoreCreateInfo semaphoreInfo{};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;


			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


			if (vkCreateSemaphore(m_device->Get(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
				vkCreateSemaphore(m_device->Get(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
				vkCreateFence(m_device->Get(), &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
				PHNX_FATAL("failed to create semaphores!");
			}
		}
	}
	std::vector<char> ReadFile(const char* filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			PHNX_FATAL("Failed To Open File '%s'", filename);
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}
}