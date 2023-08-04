#include <Phoenix/graphics/opengl/Buffer.hpp>
#include <glad/glad.h>
namespace phnx {
	namespace gfx {
		namespace buffers {

			VertexBuffer::VertexBuffer() : m_id(0)
			{
				GL_CALL(glGenBuffers(1, &m_id));
			}

			void VertexBuffer::Bind()
			{
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
			}

			void VertexBuffer::SetFloats(const float* data, size_t size)

			{
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STREAM_DRAW));
			}

			void VertexBuffer::SetFloats(const std::vector<float>& data)
			{
				SetFloats(data.data(), data.size());
			}

			void VertexBuffer::SetVertices(const std::vector<Vertex>& vertices)
			{
				SetData(vertices.data(), vertices.size() * sizeof(Vertex));
				SetLayout(0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, position));
				SetLayout(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, color));
			}

			void VertexBuffer::SetVertices(Vertex* vertices, size_t length)
			{
				SetData(vertices, length * sizeof(Vertex));
				SetLayout(0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, position));
				SetLayout(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, color));
			}

			void VertexBuffer::SetData(const void* data, size_t size)
			{
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW));
			}

			void VertexBuffer::SetLayout(int index, int size, int type, bool normalized, size_t stride, size_t offset)
			{
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
				GL_CALL(glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, stride, (void*) offset));
				GL_CALL(glEnableVertexAttribArray(index));
			}

			void VertexBuffer::Unbind()
			{
				GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}

			VertexArray::VertexArray() : m_id(0)
			{
				GL_CALL(glGenVertexArrays(1, &m_id));
			}

			void VertexArray::Bind()
			{
				GL_CALL(glBindVertexArray(m_id));
			}

			void VertexArray::Unbind()
			{
				GL_CALL(glBindVertexArray(0));
			}

			IndexBuffer::IndexBuffer() : m_id(0), m_indexCount(0)
			{
				GL_CALL(glGenBuffers(1, &m_id));
			}

			void IndexBuffer::Bind()
			{
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
			}

			void IndexBuffer::Unbind()
			{
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			}

			void IndexBuffer::SetIndices(const std::vector<uint32_t>& indices)
			{
				m_indexCount = indices.size();
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STREAM_DRAW));
			}

			void IndexBuffer::SetIndices(uint32_t* indices, size_t length)
			{
				m_indexCount = length;
				GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
				GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(uint32_t), indices, GL_STREAM_DRAW));
			}

		}
	}
}