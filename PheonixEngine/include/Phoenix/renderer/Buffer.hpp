#pragma once
#include "../Core.hpp"
#include <glm/vec3.hpp>
namespace phoenix {
	namespace gfx {

		struct Vertex {
			glm::vec3 position;
		};

		class VertexArray {
		public:
			virtual void Bind() = 0;
			virtual ~VertexArray();


		protected:
			uint32_t m_id;

		};
	}
}
