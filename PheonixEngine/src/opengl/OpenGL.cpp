#include <Phoenix/gl/OpenGL.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace phoenix {
	namespace ogl {
		void Initialize()
		{
			PHNX_LOG("Starting OpenGL");

			if (!gladLoadGL()) {
				PHNX_FATAL("Failed To Load OpenGL");
			}

			int32_t vMaj = 0, vMin = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &vMaj);
			glGetIntegerv(GL_MINOR_VERSION, &vMin);
			PHNX_LOG("Loaded OpenGL v%d.%d", vMaj, vMin);
		}
		void OpenGL::ClearColor(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
}