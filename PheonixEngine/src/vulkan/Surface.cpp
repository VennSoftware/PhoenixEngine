#include <Phoenix/vk/Surface.hpp>

namespace phoenix {
	namespace vk {
		Surface::Surface(Context* ctx) : m_ctx(ctx) {
			if (glfwCreateWindowSurface(ctx->GetInstance(), ctx->GetWindow(), nullptr, &m_surface) != VK_SUCCESS)
			{
				std::cerr << "[Phoenix] - Failed To Create Surface\n";
				exit(EXIT_FAILURE);
			}
		}

		void Surface::Destroy() {
			vkDestroySurfaceKHR(m_ctx->GetInstance(), m_surface, nullptr);
		}
	}
}