#include "GLTexture.h"

namespace spruce {

	GLTexture::GLTexture() : 
		GLItem(GLItemType::Texture, glGenTexture()) {
		setDownsamplingMode(GLSamplingMode::Lerp, false, GLSamplingMode::Lerp);
		setUpsamplingMode(GLSamplingMode::Lerp);
	}

	#define OPENGL_TEXTURE_MIPMAPS
	void GLTexture::uploadBitmap(Bitmap const& bmp) {
		panicIf(bmp.getPixelData() == nullptr, "Trying to upload initialized bitmap to GPU.");

		// Generate texture mipmaps.
		glBindTexture(GL_TEXTURE_2D, mGLId);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			(GLsizei) bmp.getWidth(),
			(GLsizei) bmp.getHeight(),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			(GLvoid const*) bmp.getPixelData()
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	void GLTexture::setDownsamplingMode(GLSamplingMode localMode, bool useMipmaps, GLSamplingMode mipmapMode) {
		glBindTexture(GL_TEXTURE_2D, mGLId);

		// Mipmaps enabled.
		if (useMipmaps) {
			switch (localMode)
			{
				// Use nearest pixel inside mip-level.
				case GLSamplingMode::Point:
				switch (mipmapMode)
				{
					// Use nearest mip-level.
					case GLSamplingMode::Point:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					break;

					// Lerp between mip-levels.
					case GLSamplingMode::Lerp:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
					break;
				}
				break;

				// Lerp between pixels inside mip-level.
				case GLSamplingMode::Lerp:
				switch (mipmapMode)
				{
					// Use nearest mip-level.
					case GLSamplingMode::Point:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					break;

					// Lerp between mip-levels.
					case GLSamplingMode::Lerp:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					break;
				}
				break;
			}
		}

		// Mipmaps disabled.
		else {
			switch (localMode)
			{
				// Use nearest pixel in mip-level 0.
				case GLSamplingMode::Point:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;

				// Lerp between pixels in mip-level 0.
				case GLSamplingMode::Lerp:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;
			}
		}
	}

	void GLTexture::setUpsamplingMode(GLSamplingMode localMode) {
		glBindTexture(GL_TEXTURE_2D, mGLId);

		switch (localMode) {
			case GLSamplingMode::Point:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;

			case GLSamplingMode::Lerp:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}
	}

	void GLTexture::setMaxAnisotropy(float max) {
		float maxSupportedAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxSupportedAnisotropy);
		panicIf(max < 1.0f || max > maxSupportedAnisotropy, "Unsupported value of max anisotropy.");
		glBindTexture(GL_TEXTURE_2D, mGLId);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
	}

	GLTexture::~GLTexture() {
		glDeleteTexture(mGLId);
	}
}