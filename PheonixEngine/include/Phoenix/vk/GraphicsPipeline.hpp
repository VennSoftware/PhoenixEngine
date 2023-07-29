#pragma once
#include "Context.hpp"


namespace phoenix {
	namespace vk {
		class GraphicsPipeline {
			
		};

		class PipelineBuilder {
		public:
			PipelineBuilder(std::shared_ptr<Device> device);
			void CreatePipeline();
			void CreateRenderPass();
			VkShaderModule CreateShaderModule(const std::vector<char>& code);
			VkPipelineLayout GetPipelineLayout() { return m_pipelineLayout; }
			VkRenderPass GetRenderPass() { return m_renderPass; }
			VkPipeline Get() { return m_graphicsPipeline; }
		protected:


		private:
			std::shared_ptr<Device> m_device;
			Context* m_ctx;
			VkPipelineLayout m_pipelineLayout;
			VkRenderPass m_renderPass;
			VkPipeline m_graphicsPipeline;
		};
	}
}