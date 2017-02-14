#include "spruce_opengl_texture.h"

spruce::OpenGL_Texture::OpenGL_Texture()
	: OpenGL_Item(OpenGL_Item_Type::Texture, glGenTexture()) {
	set_downsampling_mode(OpenGL_Sampling_Mode::Lerp, false, OpenGL_Sampling_Mode::Lerp);
	set_upsampling_mode(OpenGL_Sampling_Mode::Lerp);
}

void spruce::OpenGL_Texture::upload_bitmap_data(Bitmap const& bmp) {
	panic_if(bmp.data() == nullptr, "Trying to upload initialized bitmap to GPU.");
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		bmp.channelCount() == 3 ? GL_RGB : GL_RGBA,
		(GLsizei) bmp.width(), 
		(GLsizei) bmp.height(),
		0,
		bmp.channelCount() == 3 ? GL_RGB : GL_RGBA,
		GL_UNSIGNED_BYTE, 
		(GLvoid const*) bmp.data()
	);
}

void spruce::OpenGL_Texture::set_downsampling_mode(OpenGL_Sampling_Mode localMode, bool useMipmaps, OpenGL_Sampling_Mode mipmapMode) {
	glBindTexture(GL_TEXTURE_2D, id_);

	// Mipmaps enabled.
	if (useMipmaps) {
		switch (localMode)
		{
			// Use nearest pixel inside mip-level.
			case OpenGL_Sampling_Mode::Point:
			switch (mipmapMode)
			{
				// Use nearest mip-level.
				case OpenGL_Sampling_Mode::Point:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				break;

				// Lerp between mip-levels.
				case OpenGL_Sampling_Mode::Lerp:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				break;
			}
			break;

			// Lerp between pixels inside mip-level.
			case OpenGL_Sampling_Mode::Lerp:
			switch (mipmapMode)
			{
				// Use nearest mip-level.
				case OpenGL_Sampling_Mode::Point:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				break;

				// Lerp between mip-levels.
				case OpenGL_Sampling_Mode::Lerp:
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
			case OpenGL_Sampling_Mode::Point:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;

			// Lerp between pixels in mip-level 0.
			case OpenGL_Sampling_Mode::Lerp:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		}
	}
}

void spruce::OpenGL_Texture::set_upsampling_mode(OpenGL_Sampling_Mode localMode) {
	glBindTexture(GL_TEXTURE_2D, id_);
	
	switch (localMode) {
		case OpenGL_Sampling_Mode::Point:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;

		case OpenGL_Sampling_Mode::Lerp:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}
}

void spruce::OpenGL_Texture::set_max_anisotropy(float max) {
	float maxSupportedAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxSupportedAnisotropy);
	panic_if(max < 1.0f || max > maxSupportedAnisotropy, "Unsupported value of max anisotropy.");
	glBindTexture(GL_TEXTURE_2D, id_);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max);
}

spruce::OpenGL_Texture::~OpenGL_Texture() {
	glDeleteTexture(id_);
}
