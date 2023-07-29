#pragma once
#include "Context.hpp"

namespace phoenix {
	namespace vk {

		struct ColorRGBA {
			float r, g, b, a;
		};

		class RenderQueue {
		public:
			RenderQueue(Context* ctx);

			void BeginPass(int imageIndex, ColorRGBA color);
			void SubmitPass();


			void DrawArrays(int vertexCount);

			void SetViewPort(float x, float y, float w, float h);

			void SubmitCmdBuffer(VkSemaphore* waitSemaphores, VkPipelineStageFlags* waitStages, VkSemaphore* signalSemaphores);



		private:
			VkCommandPool m_cmdPool;
			std::shared_ptr<PhysDevice> m_physDevice;
			std::shared_ptr<Device> m_device;
			QueueFamilyIndices m_indices;
			Context* m_ctx;
			VkCommandBuffer m_cmdBuffer;
		};
	}
}