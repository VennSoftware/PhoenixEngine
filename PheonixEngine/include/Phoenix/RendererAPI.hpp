#pragma once

#include <Phoenix/Core.hpp>

namespace phoenix {
	class RendererAPI {
	public:
		static std::shared_ptr<RendererAPI> Get() { return s_renderer_api; }
		virtual void ClearColor(float r, float g, float b, float a) = 0;


	protected:

	private:
		static std::shared_ptr<RendererAPI> s_renderer_api;
	};
}