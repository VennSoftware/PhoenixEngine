#include <Phoenix/graphics/OrthoCamera.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace phnx {
	OrthographicCamera::OrthographicCamera(float size, float aspect)
		: m_width(size), m_height(size / aspect), m_aspect(aspect), m_position(0, 0, 0), m_size(size)
	{
	}
	glm::mat4 OrthographicCamera::GetViewProjection()
	{
		glm::mat4 view = glm::translate(glm::mat4(1.0f), m_position);
		return glm::ortho(0.0f, m_width, m_height, 0.0f) * glm::inverse(view);
	}


}