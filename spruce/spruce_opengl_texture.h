#pragma once

#include "spruce_opengl_item.h"
#include "spruce_bitmap.h"

namespace spruce {

	enum class OpenGL_Sampling_Mode {
		Point,
		Lerp
	};

	class OpenGL_Texture : public OpenGL_Item {
	public:
		// Generates a new OpenGL texture using default settings.
		OpenGL_Texture();

		void upload_bitmap_data(Bitmap const& bmp);
		void set_downsampling_mode(OpenGL_Sampling_Mode localMode, bool useMipmaps, OpenGL_Sampling_Mode mipmapMode);
		void set_upsampling_mode(OpenGL_Sampling_Mode localMode);
		void set_max_anisotropy(float max);

		inline void bind(uint32_t textureUnitId) { 
			glActiveTexture(GL_TEXTURE0 + textureUnitId); 
			glBindTexture(GL_TEXTURE_2D, id_);
		}

		~OpenGL_Texture();
	};
}