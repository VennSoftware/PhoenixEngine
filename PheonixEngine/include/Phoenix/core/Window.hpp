#pragma once
#include <Phoenix/Core.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace phnx {

	class Window {
	public:
		Window(int width, int height, const char* title);
		bool IsRunning();
		void PollEvents(bool wait = false, double timeout = 0.2);
		void Update();
		void Show();
		void Hide();
		GLFWwindow* Handle() const { return m_window; }
	protected:
	private:
		GLFWwindow* m_window;
		int m_width, m_height;
	};
}