#pragma once
#include <Phoenix/Core.hpp>
#include <entt/entt.hpp>

#include <Phoenix/graphics/OrthoCamera.hpp>
#include "Components.hpp"

namespace phnx {

	class Entity;

	class Scene {
	public:

		Entity CreateEntity(const std::string& name);



		void Update(float timestep);
		void Render(OrthographicCamera& camera);


		auto GetTaggedEntities() { return m_registry.view<TagComponent>(); }

	private:
		entt::registry m_registry;
		std::string m_name;

		friend class Entity;
	};
}