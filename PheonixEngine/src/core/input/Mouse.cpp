#include <Phoenix/core/input/Mouse.hpp>

namespace phnx {

	MouseData Mouse::s_data;

	void OnScroll(GLFWwindow* window, double scrollX, double scrollY) {
		Mouse::Data().scrollDeltaX = scrollX - Mouse::Data().scrollX;
		Mouse::Data().scrollDeltaY = scrollY - Mouse::Data().scrollY;
		Mouse::Data().scrollX = scrollX;
		Mouse::Data().scrollY = scrollY;
	}

	void OnMoved(GLFWwindow* window, double xpos, double ypos) {
		Mouse::Data().mouseX = xpos;
		Mouse::Data().mouseY = ypos;
	}

	void InitMouse(GLFWwindow* window)
	{
		
		Mouse::Reset();

		glfwSetScrollCallback(window, OnScroll);
		glfwSetCursorPosCallback(window, OnMoved);
	}

	void Mouse::Reset()
	{
		Mouse::Data().mouseX = 0;
		Mouse::Data().mouseY = 0;
		Mouse::Data().scrollDeltaX = 0;
		Mouse::Data().scrollDeltaY = 0;
		Mouse::Data().scrollX = 0;
		Mouse::Data().scrollY = 0;
	}
	void Mouse::ResetScroll()
	{
		Mouse::Data().scrollDeltaX = 0;
		Mouse::Data().scrollDeltaY = 0;
		Mouse::Data().scrollX = 0;
		Mouse::Data().scrollY = 0;
	}
}