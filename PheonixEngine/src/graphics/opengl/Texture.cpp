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
	if (pixels != nullptr)
		stbi_image_free(pixels);
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

void phnx::gfx::Texture::SetPixels(uint32_t width, uint32_t height, const uint8_t* pixels, ImageFormat format)
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void phnx::gfx::Texture::Destroy()
{
	glDeleteTextures(1, &m_id);
}

void phnx::gfx::Texture::Reload() {
	if (m_path.empty()) {
		return;
	}

	int width, height, depth;
	uint8_t* pixels = stbi_load(m_path.c_str(), &width, &height, &depth, 4);
	if (pixels == nullptr) {
		PHNX_WARN("Failed To Open Texture '%s'", m_path.c_str());
	}

	SetPixels(width, height, pixels);

	if (pixels != nullptr)
		stbi_image_free(pixels);
}

bool phnx::gfx::TextureLibrary::Exists(const std::string& name)
{
	return (m_textures.find(name) != m_textures.end());
}

Texture& phnx::gfx::TextureLibrary::Get(const std::string& name)
{
	if (!Exists(name)) {
		PHNX_FATAL("Texture '%s' Doesn't Exist!", name.c_str());
	}

	return m_textures[name];
}

void phnx::gfx::TextureLibrary::Put(const std::string& name, Texture texture)
{
	m_textures[name] = texture;
}

void phnx::gfx::TextureLibrary::Load(const std::string& name, const std::string& path)
{
	Put(name, Texture(path));
}
