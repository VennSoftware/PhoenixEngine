#pragma once
#include <Phoenix/renderer/Buffer.hpp>

namespace phoenix {
	namespace gfx {
		class GLVertexArray : public VertexArray {
			// Inherited via VertexArray
			virtual void Bind() override;

			virtual ~GLVertexArray() override;
		};
	}
}