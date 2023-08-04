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

		class Texture {
		public:

		private:
			uint32_t m_id, m_width, m_height;
			ImageFormat m_format;
		};
	}
}