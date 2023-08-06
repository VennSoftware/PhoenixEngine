#include <Phoenix/core/Scene.hpp>
#include <Phoenix/core/Components.hpp>
#include <Phoenix/core/Entity.hpp>
#include <Phoenix/graphics/Renderer2D.hpp>
#include <imgui.h>
#include <Phoenix/core/Profiling.hpp>
namespace phnx {
	Entity Scene::CreateEntity(const std::string& name)
	{
		PHNX_PROFILE_FUNCTION();
		Entity entity = { m_registry.create(), this };
		auto& tag = entity.AddComponent<TagComponent>();
		tag.m_tag = name.empty() ? "Entity" : name;

		entity.AddComponent<TransformComponent>();

		return entity;
	}


	void DrawEntity(Entity entity) {

	}

	void Scene::Update(float timestep)
	{

	}

	void Scene::Render(OrthographicCamera& camera)
	{
		PHNX_PROFILE_FUNCTION();
		gfx::Renderer2D::BeginScene(camera.GetViewProjection());
		auto view = m_registry.view<TransformComponent, SpriteComponent>();

		for (auto e : view) {
			Entity ent{ e, this };
			auto& tc = ent.GetComponent<TransformComponent>();
			auto& sprite = ent.GetComponent<SpriteComponent>();
			gfx::Renderer2D::DrawQuad(tc.m_position, tc.m_scale, sprite.m_texture, { sprite.m_rectPos, sprite.m_rectSize });
		}

		gfx::Renderer2D::EndScene();
	}
}

