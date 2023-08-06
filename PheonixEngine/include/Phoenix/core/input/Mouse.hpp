#pragma once

#include <Phoenix/Core.hpp>
#include <GLFW/glfw3.h>
namespace phnx {
	struct MouseData {
		float mouseX;
		float mouseY;
		float scrollDeltaX;
		float scrollDeltaY;
		float scrollX;
		float scrollY;
	};
	void InitMouse(GLFWwindow* window);
	class Mouse {
	public:

		static float PosX() { return s_data.mouseX; }
		static float PosY() { return s_data.mouseY; }
		static float ScrollDeltaX() { return s_data.scrollDeltaX; }
		static float ScrollDeltaY() { return s_data.scrollDeltaY; }

		static void Reset();
		static void ResetScroll();

		static MouseData& Data() { return s_data; }
	private:
		static MouseData s_data;
	};

	
}