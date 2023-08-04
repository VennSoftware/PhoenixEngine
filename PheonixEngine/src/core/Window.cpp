#include <Phoenix/core/Window.hpp>
#include <Phoenix/graphics/opengl/OpenGL.hpp>
static bool s_Started = false;

phnx::Window::Window(int width, int height, const char* title)
	: m_width(width), m_height(height), m_window(nullptr)
{
	if (!s_Started) {
		glfwInit();
		s_Started = true;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

	glfwMakeContextCurrent(m_window);

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
		::phnx::gfx::ogl::ResizeViewport(0, 0, width, height);
	});
}

bool phnx::Window::IsRunning() {
	return !glfwWindowShouldClose(m_window);
}

void phnx::Window::PollEvents(bool wait, double timeout) {
	if (wait) {
		glfwWaitEventsTimeout(timeout);
	} else {
		glfwPollEvents();
	}
}

void phnx::Window::Update() {
	glfwSwapBuffers(m_window);
}

void phnx::Window::Hide() {
	glfwHideWindow(m_window);
}

void phnx::Window::Show() {
	glfwShowWindow(m_window);
}
