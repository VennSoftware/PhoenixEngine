#define PHNX_USE_OPENGL
#include <Phoenix/Core.hpp>
#include <Phoenix/Window.hpp>
#include <Phoenix/RendererAPI.hpp>
#include <Phoenix/renderer/Buffer.hpp>

int main() {
	phoenix::WindowSpec spec{};
	spec.contextType = phoenix::ContextType::OpenGL;
	spec.width = 1080;
	spec.height = 720;
	spec.title = "Phoenix App";
	spec.useVsync = true;

	phoenix::Window window = phoenix::Window::Create(spec);

	phoenix::VertexArray* vao = phoenix::g_renderer_api->CreateVertexArray();

	while (window.IsActive()) {
		window.Update();
	}
}