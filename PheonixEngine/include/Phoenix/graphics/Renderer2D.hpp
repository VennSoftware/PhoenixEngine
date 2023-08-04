#pragma once

#include <Phoenix/Core.hpp>
#include <Phoenix/graphics/opengl/Buffer.hpp>
#include <Phoenix/graphics/opengl/Shader.hpp>
#include <Phoenix/graphics/opengl/Texture.hpp>
namespace phnx {
	namespace gfx {

		struct RendererStats {
			int quadCount;
			int batches;
			int timeElapsed;
		};

		class Renderer2D {
		public:
			static void Initialize();
			static void BeginScene(const glm::mat4& viewProj);
			static void EndScene();

			static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color);
			static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color);
			static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const Texture& texture);
			static void DrawQuad(const glm::vec3& position, const glm::vec3& scale, const Texture& texture, const TexRect& rect);


			static RendererStats GetStats();
			static void ResetStats();
		protected:
			static void Flush();
			static void PushVertex(const glm::vec3& position, const glm::vec3& color, const glm::vec2& uv0);
			static void PushIndex(uint32_t index);
			static void PushIndices(const std::vector<uint32_t>& indices);
			

		private:
			static std::shared_ptr<buffers::VertexArray> s_vao;
			static std::shared_ptr<buffers::VertexBuffer> s_vbo;
			static std::shared_ptr<buffers::IndexBuffer> s_ibo;
			static Shader s_shader;
			static std::vector<uint32_t> s_indices;
			static std::vector<buffers::Vertex> s_vertices;
			static glm::mat4 s_transform, s_viewProj;

			static int s_batches;
			static int s_quadCount;
			static uint32_t s_vertexCount;
			static uint32_t s_indexCount;

			static uint32_t s_counter_id;
			static uint64_t s_counter_value;

			static buffers::Vertex* s_verts;
			static uint32_t* s_tris;

			static Texture s_whiteTex;
			static Texture s_diffuse;

			static const size_t c_MAX_QUADS = 10000;
		};
	}
}