#include <Phoenix/graphics/opengl/Buffer.hpp>
#include <Phoenix/core/Profiling.hpp>
#include <glad/glad.h>
namespace phnx {
	namespace gfx {
		namespace buffers {

			VertexBuffer::VertexBuffer() : m_id(0)
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glGenBuffers(1, &m_id));
			}

			void VertexBuffer::Bind()
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
			}

			void VertexBuffer::SetFloats(const float* data, size_t size)

			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STREAM_DRAW));
			}

			void VertexBuffer::SetFloats(const std::vector<float>& data)
			{
				PHNX_PROFILE_FUNCTION();
				SetFloats(data.data(), data.size());
			}

			void VertexBuffer::SetVertices(const Vertex* vertices, size_t length)
			{
				PHNX_PROFILE_FUNCTION();
				SetData(vertices, length * sizeof(Vertex));
				SetLayout(0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, position));
				SetLayout(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, color));
				SetLayout(2, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, uv0));
			}

			void VertexBuffer::SetVertices(const std::vector<Vertex>& vertices)
			{
				PHNX_PROFILE_FUNCTION();
				SetVertices(vertices.data(), vertices.size());
			}

			

			void VertexBuffer::SetData(const void* data, size_t size)
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW));
			}

			void VertexBuffer::SetLayout(int index, int size, int type, bool normalized, size_t stride, size_t offset)
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
				GL_CALL(glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, stride, (void*) offset));
				GL_CALL(glEnableVertexAttribArray(index));
			}

			void VertexBuffer::Unbind()
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}

			VertexArray::VertexArray() : m_id(0)
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glGenVertexArrays(1, &m_id));
			}

			void VertexArray::Bind()
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindVertexArray(m_id));
			}

			void VertexArray::Unbind()
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindVertexArray(0));
			}

			IndexBuffer::IndexBuffer() : m_id(0), m_indexCount(0)
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glGenBuffers(1, &m_id));
			}

			void IndexBuffer::Bind()
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
			}

			void IndexBuffer::Unbind()
			{
				PHNX_PROFILE_FUNCTION();
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			}

			void IndexBuffer::SetIndices(const std::vector<uint32_t>& indices)
			{
				PHNX_PROFILE_FUNCTION();
				m_indexCount = indices.size();
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STREAM_DRAW));
			}

			void IndexBuffer::SetIndices(uint32_t* indices, size_t length)
			{
				PHNX_PROFILE_FUNCTION();
				m_indexCount = length;
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(uint32_t), indices, GL_STREAM_DRAW));
			}

		}
	}
}