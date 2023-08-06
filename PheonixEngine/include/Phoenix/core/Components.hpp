#pragma once
#include <Phoenix/Core.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <Phoenix/graphics/opengl/Texture.hpp>

namespace phnx {
	struct TagComponent {
		std::string m_tag;
	};

	struct TransformComponent {
		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::vec3 m_rotationEuler;

		TransformComponent(glm::vec3 pos = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }, glm::vec3 rotation = { 0, 0, 0 }) {
			m_position = pos;
			m_scale = scale;
			m_rotationEuler = rotation;
		}

		glm::mat4 GetTransform() {
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_rotationEuler.x, {1, 0, 0});
			rotation = glm::rotate(rotation, m_rotationEuler.y, { 0, 1, 0 });
			rotation = glm::rotate(rotation, m_rotationEuler.z, { 0, 0, 1 });

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);

			return (translation * rotation) * scale;
		}
	};

	struct SpriteComponent {
		gfx::Texture m_texture;
		std::string m_textureID;
		glm::vec3 m_tint;
		glm::vec2 m_rectPos;
		glm::vec2 m_rectSize;
	};
}