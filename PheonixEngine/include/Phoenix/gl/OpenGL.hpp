#pragma once

#include <Phoenix/Core.hpp>
#include <Phoenix/RendererAPI.hpp>



namespace phoenix {
	namespace ogl {
		void Initialize();

		class OpenGL : RendererAPI {
		public:
			virtual void ClearColor(float r, float g, float b, float a);
		};
	}
}