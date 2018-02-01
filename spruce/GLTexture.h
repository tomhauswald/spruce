#pragma once

#include "GLItem.h"
#include "Bitmap.h"

namespace spruce {

	enum class GLSamplingMode {
		Point,
		Lerp
	};

	class GLTexture : public GLItem {
	public:
		// Generates a new OpenGL texture using default settings.
		GLTexture();

		void uploadBitmap(Bitmap const& bmp);
		void setDownsamplingMode(GLSamplingMode localMode, bool useMipmaps, GLSamplingMode mipmapMode);
		void setUpsamplingMode(GLSamplingMode localMode);
		void setMaxAnisotropy(float max);

		inline void bind(uint32_t textureUnitId) { 
			glActiveTexture(GL_TEXTURE0 + textureUnitId); 
			glBindTexture(GL_TEXTURE_2D, mGLId);
		}

		~GLTexture();
	};
}