#include <Phoenix/core/Entity.hpp>

namespace phnx {
	Entity::Entity(entt::entity handle, Scene* scene) : m_entityHandle(handle), m_scene(scene)
	{

	}
}