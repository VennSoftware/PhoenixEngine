#include <Phoenix/Window.hpp>

#define GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include <Phoenix/gl/OpenGL.hpp>

namespace phoenix {
	bool g_inited = false;
	Window Window::Create(const WindowSpec& spec) {
		if (!g_inited) {
			if (!glfwInit()) {
				PHNX_FATAL("Failed To Initialize GLFW");
			}
			g_inited = true;
		}




		if (spec.contextType == ContextType::Vulkan || spec.contextType == ContextType::None) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		}

		GLFWwindow* window = glfwCreateWindow(spec.width, spec.height, spec.title, NULL, NULL);

		if (spec.contextType == ContextType::OpenGL) {
			glfwMakeContextCurrent(window);
			glfwSwapInterval(spec.useVsync? 1 : 0);

			ogl::Initialize();
		}
		

		PHNX_DEBUG("Created Window");

		return Window(window, spec);
	}

	bool Window::IsActive()
	{
		return !glfwWindowShouldClose(m_window);
	}

	void Window::Update()
	{
		glfwPollEvents();

		if (m_spec.contextType == ContextType::OpenGL) {
			glfwSwapBuffers(m_window);
		}
	}
}
