

#include <Phoenix/core/Application.hpp>
#include <Phoenix/graphics/Renderer2D.hpp>
#include <Phoenix/graphics/opengl/OpenGL.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/vec3.hpp>
namespace phnx {


	void Application::OnCreate()
	{

	}

	void Application::OnDestroy()
	{

	}

	void Application::OnImGui()
	{
		
	}

	void Application::Clear(float r, float g, float b, float a) {
		gfx::ogl::ClearColor(r, g, b, a);
	}

	void Application::Start(const AppSpec& spec)
	{
		m_window = std::make_unique<Window>(spec.width, spec.height, spec.title);
		gfx::InitializeOpenGL();

		gfx::Renderer2D::Initialize();

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(m_window->Handle(), true);
		ImGui_ImplOpenGL3_Init();

		OnCreate();
		m_window->Show();
		double timeStart = glfwGetTime();
		double timeEnd = timeStart;
		while (m_window->IsRunning())
		{
			timeStart = glfwGetTime();
			m_window->PollEvents();

			OnUpdate(deltaTime);
			OnRender();

			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();
			OnImGui();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			m_window->Update();
			timeEnd = glfwGetTime();
			deltaTime = timeEnd - timeStart;
		}

		OnDestroy();
	}
}