#pragma once
#include "Context.hpp"

namespace phoenix
{
	namespace vk {
		class Surface {
		public:
			Surface(Context* ctx);


			void Destroy();

			VkSurfaceKHR Get() { return m_surface; }

		private:
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			Context* m_ctx;
		};
	}
}