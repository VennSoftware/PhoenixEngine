#pragma once
#include "Core.hpp"

// Forward Decls
typedef struct GLFWwindow;

namespace phoenix {

	enum class ContextType {
		None = 0,
		OpenGL,
		Vulkan
	};

	struct WindowSpec {
		ContextType contextType;
		int width;
		int height;
		const char* title;
		bool useVsync;
	};

	class Window {
	public:
		static Window Create(const WindowSpec& spec);

		bool IsActive();

		void Update();


	protected:
		Window(GLFWwindow* window, WindowSpec spec) : m_window(window), m_spec(spec) {}

	private:
		GLFWwindow* m_window;
		WindowSpec m_spec;
	};
}