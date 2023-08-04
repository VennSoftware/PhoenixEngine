#pragma once


#include <Phoenix/Core.hpp>
#include <glm/vec3.hpp>
namespace phnx {
	namespace gfx {
		namespace buffers {

			struct Vertex {
				glm::vec3 position;
				glm::vec3 color;
			};

			class VertexBuffer
			{
			public:
				VertexBuffer();
				void Bind();
				void SetFloats(const float* data, size_t size);
				void SetFloats(const std::vector<float>& data);
				void SetVertices(const std::vector<Vertex>& vertices);
				void SetVertices(Vertex* vertices, size_t length);
				void SetData(const void* data, size_t size);
				void SetLayout(int index, int size, int type, bool normalized = false, size_t stride = 0, size_t offset = 0);
				void Unbind();
				uint32_t ID() const { return m_id; }

			private:
				uint32_t m_id;
			};

			class VertexArray
			{
			public:
				VertexArray();
				void Bind();
				void Unbind();
				uint32_t ID() const { return m_id; }
			private:
				uint32_t m_id;
			};

			class IndexBuffer
			{
			public:
				IndexBuffer();
				void Bind();
				void Unbind();
				void SetIndices(const std::vector<uint32_t>& indices);
				void SetIndices(uint32_t* indices, size_t length);
				uint32_t ID() const { return m_id; }
				uint16_t IndexCount() const { return m_indexCount; }
			private:
				uint32_t m_id;
				uint16_t m_indexCount;
			};

		}
	}
}