#include <Phoenix/core/Application.hpp>
#include <Phoenix/graphics/opengl/OpenGL.hpp>
#include <Phoenix/graphics/opengl/Shader.hpp>
#include <imgui.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Phoenix/graphics/Renderer2D.hpp>
#include <Phoenix/graphics/OrthoCamera.hpp>
using namespace phnx::gfx::buffers;
using namespace phnx::gfx;
class PhoenixApp : public phnx::Application {
	
	glm::vec3 bg = { 0.1f, 0.2f, 0.3f };
	glm::vec3 triColor = {1, 1, 1};
	std::shared_ptr<VertexBuffer> vbo;
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<IndexBuffer> ibo;
	std::shared_ptr<phnx::gfx::Shader> shader;

	glm::vec3 position = { 0, 0, 0 }, scale = {1, 1, 1};
	glm::mat4 transform = glm::mat4(1.0f);
	float rotation = 0.0f;

	int quadCount = 100;

	std::shared_ptr<phnx::OrthographicCamera> m_camera;

	void updateTransform() {
	}


	virtual void OnCreate() override {
		m_camera = std::make_shared<phnx::OrthographicCamera>(50.0f, 16.0f / 9.0f);
	}

	virtual void OnUpdate(float timestep) override {
		auto window = m_window->Handle();

		

		if (glfwGetKey(window, GLFW_KEY_W)) {
			m_camera->GetPosition().y -= 1.0f * timestep;
		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			m_camera->GetPosition().y += 1.0f * timestep;
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			m_camera->GetPosition().x -= 1.0f * timestep;
		}

		if (glfwGetKey(window, GLFW_KEY_D)) {
			m_camera->GetPosition().x += 1.0f * timestep;
		}

		if (glfwGetKey(window, GLFW_KEY_E)) {
			float size = m_camera->GetSize();
			float deltaSize = size * 0.1f;
			size += deltaSize * timestep;
			m_camera->SetSize(size);
		}

		if (glfwGetKey(window, GLFW_KEY_Q)) {
			float size = m_camera->GetSize();
			float deltaSize = -(size * 0.1f);
			size += deltaSize * timestep;
			if (size <= 0.1f) { size = 0.1f; }
			m_camera->SetSize(size);
		}
	}

	virtual void OnRender() override {
		Clear(bg.r, bg.g, bg.b, 1);
		Renderer2D::BeginScene(m_camera->GetViewProjection());
		float viewWidth, viewHeight;
		viewWidth = m_camera->Width();
		viewHeight = m_camera->Height();
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				Renderer2D::DrawQuad(
					{ (float)x + viewWidth / 4, (float)y + viewHeight / 4, 0.1f }, { 0.90f, 0.90f, 1.0f }, { x / 10.0f, y / 10.0f, 1.0f}
				);
			}
		}
		Renderer2D::EndScene();
	}

	virtual void OnImGui() override {
		ImGui::Begin("Stats & Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("DeltaTime: %02.02f ms (%d fps)", DeltaTime() * 1000.0f, (int) (1.0f / DeltaTime()));
		ImGui::ColorEdit3("Background Color", &bg.x);

		ImGui::DragInt("Quad Count", &quadCount, 1.0f, 0, 10000);

		if (ImGui::ColorEdit3("Triangle Color", &triColor.x)) {
		}

		if (ImGui::DragFloat3("Position", &position[0], 0.01f, 0.0f, 10.0f)) {
			updateTransform();
		}

		if (ImGui::DragFloat3("Scale", &scale[0], 0.01f, 0.0f, 10.0f)) {
			updateTransform();
		}
		ImGui::BeginDisabled();
		if (ImGui::DragFloat3("Camera Pos", &(m_camera->GetPosition().x), 0.01f, 0.0f, 10.0f)) {
			updateTransform();
		}

		ImGui::Text("Camera Scale: %f", m_camera->GetSize());
		ImGui::EndDisabled();

		float rot = glm::degrees(rotation);
		if (ImGui::DragFloat("Rotation", &rot, 1, 0.0f, 360.0f)) {
			rotation = glm::radians(rot);
			updateTransform();
		}
		ImGui::NewLine();
		ImGui::SeparatorText("Renderer Stats");
		RendererStats stats = Renderer2D::GetStats();
		ImGui::Text("Batches: %d", stats.batches);
		ImGui::Text("Quads: %d", stats.quadCount);
		
		ImGui::Text("Max Texture Slots: %d", ogl::MaxTextureUnits());
		ImGui::Text("Max Vertex Attributes: %d", ogl::MaxVertexAttribs());
		ImGui::Text("GLSL Version: %s", ogl::GLSLVersion());
		ImGui::Text("VRAM Usage %d/%d MB", ogl::MemoryUsed() / 1024, ogl::MemoryTotal() / 1024);
		Renderer2D::ResetStats();

		ImGui::End();
	}


};


int main() {
	phnx::AppSpec spec{};

	spec.width = 1280;
	spec.height = 720;
	spec.title = "Phoenix App";

	PhoenixApp app;

	app.Start(spec);

	return 0;
}