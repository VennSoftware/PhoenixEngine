#include <Phoenix/graphics/Renderer2D.hpp>
#include <Phoenix/graphics/opengl/OpenGL.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace phnx::gfx;

std::shared_ptr<phnx::gfx::buffers::VertexArray> phnx::gfx::Renderer2D::s_vao;
std::shared_ptr<phnx::gfx::buffers::VertexBuffer> phnx::gfx::Renderer2D::s_vbo;
std::shared_ptr<phnx::gfx::buffers::IndexBuffer> phnx::gfx::Renderer2D::s_ibo;
phnx::gfx::Shader phnx::gfx::Renderer2D::s_shader;
std::vector<uint32_t> phnx::gfx::Renderer2D::s_indices;
std::vector<phnx::gfx::buffers::Vertex> phnx::gfx::Renderer2D::s_vertices; 
glm::mat4 phnx::gfx::Renderer2D::s_transform, phnx::gfx::Renderer2D::s_viewProj;

int phnx::gfx::Renderer2D::s_batches;
int phnx::gfx::Renderer2D::s_quadCount;
uint32_t phnx::gfx::Renderer2D::s_counter_id;
uint64_t phnx::gfx::Renderer2D::s_counter_value;

uint32_t phnx::gfx::Renderer2D::s_indexCount;
uint32_t phnx::gfx::Renderer2D::s_vertexCount;

uint32_t* phnx::gfx::Renderer2D::s_tris;
phnx::gfx::buffers::Vertex* phnx::gfx::Renderer2D::s_verts;

struct GLQueryTimer {
	uint32_t m_startID, m_endID;

};


void phnx::gfx::Renderer2D::Initialize()
{
	glGenQueries(1, &s_counter_id);

	s_vao = std::make_shared<buffers::VertexArray>();
	s_vbo = std::make_shared<buffers::VertexBuffer>();
	s_ibo = std::make_shared<buffers::IndexBuffer>();

	s_vao->Bind();
	s_vbo->Bind();
	s_ibo->Bind();

	s_shader = *LoadShader("assets/shaders/batched.vert", "assets/shaders/batched.frag").get();

	s_verts = (buffers::Vertex*) std::malloc(sizeof(buffers::Vertex) * 4 * c_MAX_QUADS);
	s_tris = (uint32_t*) std::malloc(sizeof(uint32_t) * 6 * c_MAX_QUADS);
}

void phnx::gfx::Renderer2D::BeginScene(const glm::mat4& viewProj)
{
	
	s_indices.clear();
	s_vertices.clear();

	s_viewProj = viewProj;
}

void phnx::gfx::Renderer2D::EndScene() {
	Flush();
	

	

	

}

void phnx::gfx::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color)
{
	s_quadCount++;
	if (s_vertexCount > c_MAX_QUADS * 4) { Flush(); }

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);
	s_transform = translation * scaling;
	glm::vec3 bottomRight = {  0.5f, -0.5f, 0.0f };
	glm::vec3 topRight	  = {  0.5f,  0.5f, 0.0f };
	glm::vec3 topLeft	  = { -0.5f,  0.5f, 0.0f };
	glm::vec3 bottomLeft  = { -0.5f, -0.5f, 0.0f };
	glm::vec3 c = glm::vec3(color);

	PushIndices({ 0, 1, 2, 2, 3, 0 });

	PushVertex(bottomRight, c);
	PushVertex(topRight,    c);
	PushVertex(topLeft,     c);
	PushVertex(bottomLeft,  c);

	
}

void phnx::gfx::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color)
{
	DrawQuad(position, scale, glm::vec4(color, 1.0f));
}

void phnx::gfx::Renderer2D::Flush()
{
	s_batches++;
	s_vbo->SetVertices(s_verts, s_vertexCount);
	s_ibo->SetIndices(s_tris, s_indexCount);
	s_shader.Use();
	ogl::DrawIndexed(s_vao, s_ibo);
	/*int done = 0;
	while (!done) {
		glGetQueryObjectiv(s_counter_id,
			GL_QUERY_RESULT_AVAILABLE,
			&done);
	}*/
	uint64_t elapsed = 0;
	//glGetQueryObjectui64v(s_counter_id, GL_QUERY_RESULT, &elapsed);
	s_counter_value += elapsed;
	s_vertexCount = 0;
	s_indexCount = 0;
}

void phnx::gfx::Renderer2D::PushVertex(const glm::vec3& position, const glm::vec3& color)
{
	glm::vec4 tPos = s_viewProj * s_transform * glm::vec4(position, 1.0f);
	//tPos /= tPos.w;
	s_verts[s_vertexCount++] = { {tPos.x, tPos.y, tPos.z}, color };
}

void phnx::gfx::Renderer2D::PushIndex(uint32_t index)
{
	size_t base = s_indices.size();
	s_indices.push_back(index + base);
}

void phnx::gfx::Renderer2D::PushIndices(const std::vector<uint32_t>& indices)
{
	size_t base = s_vertexCount;
	for (uint32_t index : indices)
	{
		s_tris[s_indexCount++] = index + base;
	}
}

phnx::gfx::RendererStats phnx::gfx::Renderer2D::GetStats()
{
	RendererStats stats{};
	stats.batches = s_batches;
	stats.quadCount = s_quadCount;
	stats.timeElapsed = s_counter_value;
	return stats;
}

void phnx::gfx::Renderer2D::ResetStats()
{
	s_batches = 0;
	s_quadCount = 0;
	s_counter_value = 0;
}


