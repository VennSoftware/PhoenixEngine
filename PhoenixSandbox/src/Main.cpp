#define PHNX_USE_OPENGL
#include <Phoenix/Core.hpp>
#include <Phoenix/Window.hpp>

int main() {
	phoenix::WindowSpec spec{};
	spec.contextType = phoenix::ContextType::OpenGL;
	spec.width = 1080;
	spec.height = 720;
	spec.title = "Phoenix App";
	spec.useVsync = true;

	phoenix::Window window = phoenix::Window::Create(spec);

	while (window.IsActive()) {
		window.Update();
	}
}