#include "SceneListPanel.hpp"
#include <imgui.h>
#include <Phoenix/graphics/Renderer2D.hpp>
namespace phnx {
	 
	phnx::SceneListPanel::SceneListPanel(std::shared_ptr<Scene> scene)
	{
		SetScene(scene);
	}

	void SceneListPanel::SetScene(std::shared_ptr<Scene> scene)
	{
		m_scene = scene;
		m_currentEntity = nullptr;
	}

	void SceneListPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy");
			if (m_currentEntity && m_currentEntity->IsValid()) {
		
			ImGui::SameLine();

			if (ImGui::Button("Add Entity")) {
				m_currentEntity = new Entity(m_scene->CreateEntity("Empty Entity"));
			}


		
				DrawSelectedEntity();
			}

		ImGui::End();
	}

	void SceneListPanel::SetSelectedEntity(Entity* entity)
	{
		m_currentEntity = entity;
	}

	void SceneListPanel::DrawSelectedEntity()
	{

		if (m_currentEntity->HasComponent<TagComponent>()) {
			auto& tag = m_currentEntity->GetComponent<TagComponent>().m_tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (m_currentEntity->HasComponent<TransformComponent>()) {
			auto& tc = m_currentEntity->GetComponent<TransformComponent>();
			ImGui::SeparatorText("Transform");
			ImGui::DragFloat3("Position", &tc.m_position[0]);
			ImGui::DragFloat3("Scale", &tc.m_scale[0]);
			ImGui::DragFloat3("Rotation", &tc.m_rotationEuler[0]);
		}

		if (m_currentEntity->HasComponent<SpriteComponent>()) {
			auto& sc = m_currentEntity->GetComponent<SpriteComponent>();
			ImGui::SeparatorText("Sprite Component");
			ImGui::Text("Texture: %s", sc.m_texture.Path().c_str());
			ImGui::Text("Texture Size: %dx%d", sc.m_texture.Width(), sc.m_texture.Height());

			auto& textures = gfx::Renderer2D::GetTextureLibrary();
			
			
			if (ImGui::BeginCombo("Texture", sc.m_textureID.c_str())) {
				for (auto& name : textures.TextureMap()) {
					bool selected = name.first == sc.m_textureID;
					if (ImGui::Selectable(name.first.c_str(), &selected)) {
						sc.m_texture = textures.Get(name.first);
						sc.m_textureID = name.first;
					}
				}
				ImGui::EndCombo();
			}

			ImGui::SliderFloat("Preview Scale", &m_spriteScale, 0.1f, 2.0f, "%02.1fx");
			float width = sc.m_texture.Width() * m_spriteScale;
			float height = sc.m_texture.Height() * m_spriteScale;
			ImGui::DragFloat2("Rect Pos", &sc.m_rectPos[0], 1.0f, 0, FLT_MAX);
			ImGui::DragFloat2("Rect Size", &sc.m_rectSize[0], 1.0f, 0, FLT_MAX);
			ImVec2 imagePos = ImGui::GetCursorScreenPos();
			ImGui::Image((ImTextureID)sc.m_texture.Handle(), {width , height });
			ImVec2 begin = { sc.m_rectPos.x * m_spriteScale, sc.m_rectPos.y * m_spriteScale };
			begin.x += imagePos.x;
			begin.y += imagePos.y;
			ImVec2 end = { begin.x + sc.m_rectSize.x * m_spriteScale, begin.y + sc.m_rectSize.y * m_spriteScale };
			ImGui::GetWindowDrawList()->AddRect(begin, end, ImColor(255, 255, 255));
			
		}

		if (ImGui::BeginPopupContextWindow()) {
			if (!m_currentEntity->HasComponent<SpriteComponent>()) {
				if (ImGui::Button("Add Sprite Component")) {
					auto& sprite = m_currentEntity->AddComponent<SpriteComponent>();
					sprite.m_texture = gfx::Renderer2D::GetTextureLibrary().Get("white");
					sprite.m_rectSize = { sprite.m_texture.Width(), sprite.m_texture.Height() };
				}
			}
			ImGui::EndPopup();
		}
	}
	
}
