#pragma once

#include <Phoenix/core/Scene.hpp>
#include <Phoenix/core/Entity.hpp>
#include <Phoenix/core/Components.hpp>

namespace phnx {
	class SceneListPanel { 
	public:
		SceneListPanel(std::shared_ptr<Scene> scene);
		void SetScene(std::shared_ptr<Scene> scene);
		void OnImGuiRender();

		void SetSelectedEntity(Entity* entity);

	private:
		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);

		void DrawSelectedEntity();

	private:
		Entity* m_currentEntity;
		std::shared_ptr<Scene> m_scene;

		float m_spriteScale = 1.0f;
	};
}