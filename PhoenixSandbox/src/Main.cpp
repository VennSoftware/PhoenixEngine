#include <Phoenix.hpp>
#include <Phoenix/core/Profiling.hpp>
#include "panels/Panels.hpp"

using namespace phnx::gfx::buffers;
using namespace phnx::gfx;
class PhoenixApp : public phnx::Application {
	
	glm::vec3 bg = { 0.1f, 0.2f, 0.3f };
	glm::vec3 triColor = {1, 1, 1};

	Texture m_diffuse;

	std::shared_ptr<phnx::OrthographicCamera> m_camera;
	std::shared_ptr<phnx::Scene> m_scene;
	std::shared_ptr<phnx::SceneListPanel> m_sceneList;

	void updateTransform() {
	}


	virtual void OnCreate() override {
		m_camera = std::make_shared<phnx::OrthographicCamera>(10.0f, 16.0f / 9.0f);

		Renderer2D::EnableBlending();
		Renderer2D::GetTextureLibrary().Load("aliens", "assets/textures/aliens.png");

		m_scene = std::make_shared<phnx::Scene>();

		phnx::Entity ent = m_scene->CreateEntity("Hello");

		auto& sprite = ent.AddComponent<phnx::SpriteComponent>();
		sprite.m_texture = phnx::gfx::Renderer2D::GetTextureLibrary().Get("aliens");
		sprite.m_rectSize = { sprite.m_texture.Width(), sprite.m_texture.Height() };

		ent.GetComponent<phnx::TransformComponent>().m_position += glm::vec3(5, 3, 0);

	}

	virtual void OnUpdate(float timestep) override {

		PHNX_PROFILE_FUNCTION();
		auto window = m_window->Handle();

		

		if (glfwGetKey(window, GLFW_KEY_W)) {
			m_camera->GetPosition().y -= (1.0f * m_camera->GetSize() / 10.0f) * timestep;
		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			m_camera->GetPosition().y += (1.0f * m_camera->GetSize() / 10.0f) * timestep;
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			m_camera->GetPosition().x -= (1.0f * m_camera->GetSize() / 10.0f) * timestep;
		}

		if (glfwGetKey(window, GLFW_KEY_D)) {
			m_camera->GetPosition().x += (1.0f * m_camera->GetSize() / 10.0f) * timestep;
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

		if (phnx::Mouse::ScrollDeltaY() < 0) {
			float size = m_camera->GetSize();
			float deltaSize = -(size * 0.1f);
			size += deltaSize;
			if (size <= 0.1f) { size = 0.1f; }
			m_camera->SetSize(size);
		}

		if (phnx::Mouse::ScrollDeltaY() > 0) {
			float size = m_camera->GetSize();
			float deltaSize = (size * 0.1f);
			size += deltaSize;
			m_camera->SetSize(size);
		}

		
	}

	virtual void OnRender() override {
		Clear(bg.r, bg.g, bg.b, 1);
		m_scene->Render(*m_camera);

		Renderer2D::DisableBlending();
	}

	virtual void OnImGui() override {

		PHNX_PROFILE_FUNCTION();
		ImGui::Begin("Stats & Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("DeltaTime: %02.02f ms (%d fs)", DeltaTime() * 1000.0f, (int) (1.0f / DeltaTime()));
		ImGui::SeparatorText("Renderer Stats");
		RendererStats stats = Renderer2D::GetStats();
		ImGui::Text("Batches: %d", stats.batches);
		ImGui::Text("Quads: %d", stats.quadCount);
		
		ImGui::Text("Max Texture Slots: %d", ogl::MaxTextureUnits());
		ImGui::Text("Max Vertex Attributes: %d", ogl::MaxVertexAttribs());
		ImGui::Text("GLSL Version: %s", ogl::GLSLVersion());
		ImGui::Text("VRAM Usage %d/%d MB", ogl::MemoryUsed() / 1024, ogl::MemoryTotal() / 1024);

		Renderer2D::ResetStats();

		m_scene->Update(1.0f);

		ImGui::End();

		ImGui::Begin("Texture List");

		ImVec2 regionAvail = ImGui::GetContentRegionAvail();
		TextureLibrary lib = Renderer2D::GetTextureLibrary();

		for (auto& texture : lib.TextureMap()) {
			const std::string& name = texture.first;
			ImGui::Text("%s", name.c_str());
			ImGui::SameLine(regionAvail.x -((ImGui::GetFontSize() * 3)));
			if (ImGui::Button("RELOAD")) {
				texture.second.Reload();
			}
			ImGui::Image((ImTextureID)texture.second.Handle(), {std::max((float)texture.second.Width(), 32.0f), std::max((float) texture.second.Height(), 32.0f)});
		}

		ImGui::End();

	

		phnx::Mouse::ResetScroll();


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