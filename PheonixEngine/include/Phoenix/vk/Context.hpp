#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <Phoenix/Core.hpp>

namespace phoenix {
	namespace vk {
		namespace prefs {
			const VkFormat FORMAT = VK_FORMAT_R8G8B8A8_SRGB;
			const VkColorSpaceKHR COLOR_SPACE = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			const VkPresentModeKHR PRESENT_MODE = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		}

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		//Forward Decls
		class PhysDevice;
		class Device;
		class Surface;
		class PipelineBuilder;
		class GraphicsPipeline;
		class RenderQueue;


		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};


#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		struct ContextSpec {
			int width;
			int height;
			const char* appName;
			const char* title;
			const char* engineName;
		};

		class Context {
		public:
			Context(const ContextSpec& spec);
			void InitializeWindow();
			void InitializeVulkan();
			void Destroy();

			int width() { return m_width; }
			int height() { return m_height; }

			bool IsActive();
			void PollEvents();

			void StartFrame();
			void EndFrame();

			

			std::vector<VkLayerProperties> LayerProps();

			VkInstance GetInstance() { return instance; }
			GLFWwindow* GetWindow() { return m_window; }
			std::shared_ptr<Surface> GetVKSurface() { return m_surface; }
			std::shared_ptr<PhysDevice> GetPhysDevice() { return m_physDevice; }
			std::shared_ptr<Device> GetDevice() { return m_device; }
			std::shared_ptr<PipelineBuilder> GetPipeline() { return m_pipeline; }
			std::shared_ptr<RenderQueue> GetRenderQueue() { return m_renderQueue; }

			std::vector<VkFramebuffer>& GetFrameBuffers() { return swapChainFramebuffers; }
			bool framebufferResized = false;
		protected:
			void CreateInstance();
			bool CheckValidationLayerSupport();
			void CreateFrameBuffers();
			void CreateSyncObjects();


		private:
			int m_width, m_height;
			GLFWwindow* m_window;
			ContextSpec m_spec;
			

			// Vulkan Variables
			VkInstance instance;
			VkSemaphore imageAvailableSemaphore;
			VkSemaphore renderFinishedSemaphore;
			VkFence inFlightFence;

			std::shared_ptr<PhysDevice> m_physDevice;
			std::shared_ptr<Device> m_device;
			std::shared_ptr<Surface> m_surface;
			std::shared_ptr<PipelineBuilder> m_pipeline;
			std::shared_ptr<RenderQueue> m_renderQueue;

			std::vector<VkFramebuffer> swapChainFramebuffers;

			uint32_t imageIndex = 0;


			friend RenderQueue;
			friend Device;
		};
	}


	std::vector<char> ReadFile(const char* filename);

}



