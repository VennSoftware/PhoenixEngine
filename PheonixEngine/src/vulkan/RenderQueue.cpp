#include <Phoenix/vk/RenderQueue.hpp>
#include <Phoenix/vk/PhysDevice.hpp>
#include <Phoenix/vk/Device.hpp>
#include <Phoenix/vk/GraphicsPipeline.hpp>
namespace phoenix {
	namespace vk {
		RenderQueue::RenderQueue(Context* ctx)
			: m_ctx(ctx), m_physDevice(ctx->GetPhysDevice()), m_indices(ctx->GetPhysDevice()->GetIndices()), m_device(ctx->GetDevice())
		{
			VkCommandPoolCreateInfo poolInfo{};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			poolInfo.queueFamilyIndex = m_indices.graphicsFamily.value();

			if (vkCreateCommandPool(m_device->Get(), &poolInfo, nullptr, &m_cmdPool) != VK_SUCCESS) {
				PHNX_FATAL("failed to create command pool!");
			}

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_cmdPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(m_device->Get(), &allocInfo, &m_cmdBuffer) != VK_SUCCESS) {
				PHNX_FATAL("failed to allocate command buffers!");
			}

		}
		void RenderQueue::BeginPass(int imageIndex, ColorRGBA c)
		{
			vkResetCommandBuffer(m_cmdBuffer, 0);
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_ctx->GetPipeline()->GetRenderPass();
			renderPassInfo.framebuffer = m_ctx->GetFrameBuffers()[imageIndex];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_device->GetExtent();

			VkClearValue clearColor = { {c.r, c.g, c.b, 1.0f} };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0; // Optional
			beginInfo.pInheritanceInfo = nullptr; // Optional

			if (vkBeginCommandBuffer(m_cmdBuffer, &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			
			vkCmdBeginRenderPass(m_cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_ctx->GetPipeline()->Get());
		}

		void RenderQueue::SubmitPass()
		{
			vkCmdEndRenderPass(m_cmdBuffer);
			if (vkEndCommandBuffer(m_cmdBuffer) != VK_SUCCESS) {
				PHNX_WARN("failed to record command buffer!");
			}
		}
		void RenderQueue::DrawArrays(int vertexCount)
		{
			vkCmdDraw(m_cmdBuffer, vertexCount, 1, 0, 0);
		}

		void RenderQueue::SetViewPort(float x, float y, float w, float h)
		{
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = w;
			viewport.height = h;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			vkCmdSetViewport(m_cmdBuffer, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = m_device->m_extent;
			vkCmdSetScissor(m_cmdBuffer, 0, 1, &scissor);
		}

		void RenderQueue::SubmitCmdBuffer(
			VkSemaphore* waitSemaphores, VkPipelineStageFlags* waitStages, VkSemaphore* signalSemaphores
		)
		{
			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_cmdBuffer;
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			if (vkQueueSubmit(m_device->m_graphicsQueue, 1, &submitInfo, m_ctx->inFlightFence) != VK_SUCCESS) {
				throw std::runtime_error("failed to submit draw command buffer!");
			}
		}
	}
}