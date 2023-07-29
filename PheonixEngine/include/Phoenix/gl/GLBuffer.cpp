#include "GLBuffer.hpp"
#include <glad/glad.h>


namespace phoenix {
	namespace gfx {
		void GLVertexArray::Bind()
		{
			glBindVertexArray(m_id);
		}

		GLVertexArray::~GLVertexArray() {
			glDeleteVertexArrays(1, &m_id);
		}


	}
}

