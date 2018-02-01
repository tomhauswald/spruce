#pragma once

#include "GLItem.h"
#include "GLTexture.h"

namespace spruce {

	class GLFramebuffer : public GLItem {
	private:
		std::vector<GLenum> mAttachments;

	public:
		GLFramebuffer()
			: GLItem(GLItemType::Framebuffer, glGenFramebuffer()) {
		}

		inline void bind() { 
			glBindFramebuffer(GL_FRAMEBUFFER, mGLId); 
		}

		void clear(fvec3 const& color);
		void addTextureAttachment(GLenum attachment, GLTexture const& texture);
	};
}