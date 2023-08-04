#pragma once
#include <Phoenix/Core.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Phoenix/graphics/opengl/Buffer.hpp>
#include <Phoenix/graphics/opengl/Shader.hpp>

namespace phnx {
	namespace gfx {


		void InitializeOpenGL();
		namespace ogl {
			void ClearColor(float r, float g, float b, float a);

			void DrawArrays(gfx::buffers::VertexArray& vao, size_t count);
			void DrawArrays(std::shared_ptr<gfx::buffers::VertexArray> vao, size_t count);
			void DrawIndexed(std::shared_ptr<gfx::buffers::VertexArray> vao, std::shared_ptr<gfx::buffers::IndexBuffer> ibo);

			int32_t MaxTextureUnits();
			int32_t MaxVertexAttribs();
			int32_t VersionMajor();
			int32_t VersionMinor();

			int32_t MemoryTotal();
			int32_t MemoryUsed();

			const uint8_t* GLSLVersion();
			const uint8_t* Vendor();
			const uint8_t* Renderer();

			const uint8_t* GetErrorName(uint32_t code);

			void ResizeViewport(int x, int y, int w, int h);

		
		}
	}
}