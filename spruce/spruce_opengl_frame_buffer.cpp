#include "spruce_opengl_frame_buffer.h"

namespace spruce {
	void OpenGL_Frame_Buffer::attach_texture(GLenum attachment, OpenGL_Texture const& texture) {
		bind();
		attachments_.push_back(attachment);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.id(), 0);
		glDrawBuffers(attachments_.size(), &attachments_[0]);
	}

	void OpenGL_Frame_Buffer::clear(glm::fvec3 const& color) {
		glClearColor(color.r, color.g, color.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}