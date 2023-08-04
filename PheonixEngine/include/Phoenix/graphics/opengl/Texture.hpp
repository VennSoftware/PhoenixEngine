#pragma once
#include <Phoenix/Core.hpp>

namespace phnx {
	namespace gfx {

		enum class ImageFormat {
			// Color Formats
			RGBA,		// Color + Alpha, uint8_t * 4
			RGB,		// Color, uint8_t * 3
			GRAYSCALE	// Grayscale (RGB = R, Alpha = 1.0f)
		};

		struct TexRect {
			glm::vec2 pos;
			glm::vec2 size;
		};

		class Texture {
		public:
			

			Texture() : m_id(0), m_width(0), m_height(0), m_format(ImageFormat::RGBA) {}
			Texture(const std::string& path);
			Texture(uint32_t width, uint32_t height, const uint8_t* pixels, ImageFormat format = ImageFormat::RGBA);

			void Attach(int unit);

			uint32_t Handle() const { return m_id; }
			uint32_t Width()  const { return m_width; }
			uint32_t Height() const { return m_height; }
			const std::string& Path() const { return m_path; }

		private:
			uint32_t m_id, m_width, m_height;
			ImageFormat m_format;
			std::string m_path;
		};
	}
}