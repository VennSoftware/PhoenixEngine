#pragma once

#include <Phoenix/Core.hpp>

namespace phnx {
	class OrthographicCamera {
	public:
		OrthographicCamera(float size, float aspect);
		glm::mat4 GetViewProjection();
		glm::vec3& GetPosition() { return m_position; }
		float GetSize() const { return m_size; }
		void SetSize(float newSize) { m_width = newSize; m_height = newSize / m_aspect; m_size = newSize; }

		float Width() const { return m_width; }
		float Height() const { return m_height; }
	private:
		float m_aspect, m_height, m_width, m_size;
		glm::vec3 m_position;
	};
}