#include <Phoenix/graphics/opengl/OpenGL.hpp>
#include <Phoenix/core/Profiling.hpp>


void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		if (id != 0x502)
			PHNX_ERROR("GL ERROR 0x%x: %s",id, message);
		break;
	case GL_DEBUG_TYPE_PERFORMANCE: PHNX_WARN("GL PERF 0x%x: %s", id, message); break;
	}
}
void phnx::gfx::InitializeOpenGL()
{
	{
		PHNX_PROFILE_FUNCTION();
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PHNX_LOG("OpenGL v%d.%d Loaded",
			ogl::VersionMajor(), ogl::VersionMinor());
		PHNX_LOG("Vendor: %s", ogl::Vendor());
		PHNX_LOG("Renderer: %s", ogl::Renderer());
#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
#endif
	}
}

void phnx::gfx::ogl::ClearColor(float r, float g, float b, float a)
{
	PHNX_PROFILE_FUNCTION();
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void phnx::gfx::ogl::DrawArrays(gfx::buffers::VertexArray& vao, size_t count)
{
	PHNX_PROFILE_FUNCTION();
	vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
	vao.Unbind();
}


void phnx::gfx::ogl::DrawArrays(std::shared_ptr<gfx::buffers::VertexArray> vao, size_t count)
{
	PHNX_PROFILE_FUNCTION();
	vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
	vao->Unbind();
}

void phnx::gfx::ogl::DrawIndexed(std::shared_ptr<gfx::buffers::VertexArray> vao, std::shared_ptr<gfx::buffers::IndexBuffer> ibo)
{
	PHNX_PROFILE_FUNCTION();
	vao->Bind();
	ibo->Bind();
	glDrawElements(GL_TRIANGLES, ibo->IndexCount(), GL_UNSIGNED_INT, nullptr);
	vao->Unbind();
}

int32_t phnx::gfx::ogl::MaxTextureUnits()
{
	int32_t x;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &x);
	return x;
}

int32_t phnx::gfx::ogl::MaxVertexAttribs()
{
	int32_t x;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &x);
	return x;
}

int32_t phnx::gfx::ogl::VersionMajor()
{
	int vMaj;
	glGetIntegerv(GL_MAJOR_VERSION, &vMaj);
	return vMaj;
}

int32_t phnx::gfx::ogl::VersionMinor()
{
	int vMin;
	glGetIntegerv(GL_MINOR_VERSION, &vMin);
	return vMin;
}

int32_t phnx::gfx::ogl::MemoryTotal()
{
	int vMin;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vMin);
	return vMin;
}

int32_t phnx::gfx::ogl::MemoryUsed()
{
	int vMin;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vMin);
	return MemoryTotal() - vMin;
}

const uint8_t* phnx::gfx::ogl::GLSLVersion()
{
	return glGetString(GL_SHADING_LANGUAGE_VERSION);
}

const uint8_t* phnx::gfx::ogl::Vendor()
{
	return glGetString(GL_VENDOR);
}

const uint8_t* phnx::gfx::ogl::Renderer()
{
	return glGetString(GL_RENDERER);
}

const uint8_t* phnx::gfx::ogl::GetErrorName(uint32_t code)
{
	return glGetString(code);
}

void phnx::gfx::ogl::ResizeViewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

