#include <Phoenix/graphics/opengl/Texture.hpp>
#include <stb/stb_image.h>
#include <glad/glad.h>
using namespace phnx::gfx;

Texture::Texture(const std::string& path)
{
	int width, height, depth;
	uint8_t* pixels = stbi_load(path.c_str(), &width, &height, &depth, 4);
	if (pixels == nullptr) {
		PHNX_WARN("Failed To Open Texture '%s'", path.c_str());
	}

	m_width = width;
	m_height = height;
	m_format = ImageFormat::RGBA;
	m_path = path;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

phnx::gfx::Texture::Texture(uint32_t width, uint32_t height, const uint8_t* pixels, ImageFormat format)
	: m_id(0), m_width(width), m_height(height), m_format(format)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void phnx::gfx::Texture::Attach(int unit)
{
	glBindTextureUnit(unit, m_id);
}
