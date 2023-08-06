#pragma once
#include <Phoenix/core/Scene.hpp>
#include <entt/entt.hpp>

namespace phnx {
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			T& comp = m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			return comp;
		}


		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.has<T>(m_entityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			if (!HasComponent<T>()) { PHNX_ERROR("Entity Is Missing Component!"); }
			return m_scene->m_registry.get<T>(m_entityHandle);
		}


		bool IsValid() { return m_scene != nullptr && m_entityHandle != entt::null; }

		entt::entity Handle() const { return m_entityHandle; }


	private:
		entt::entity m_entityHandle { entt::null };
		Scene* m_scene = nullptr;


	};
}
