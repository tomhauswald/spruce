#include "GLFramebuffer.h"

namespace spruce {
	
	void GLFramebuffer::addTextureAttachment(GLenum attachment, GLTexture const& texture) {
		bind();
		mAttachments.push_back(attachment);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.getGLId(), 0);
		glDrawBuffers(mAttachments.size(), &mAttachments[0]);
	}

	void GLFramebuffer::clear(glm::fvec3 const& color) {
		glClearColor(color.r, color.g, color.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}