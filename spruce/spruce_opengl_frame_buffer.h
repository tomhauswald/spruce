#pragma once

#include "spruce_opengl_item.h"
#include "spruce_opengl_texture.h"

namespace spruce {
	class OpenGL_Frame_Buffer : public OpenGL_Item {
	private:
		std::vector<GLenum> attachments_;

	public:
		OpenGL_Frame_Buffer()
			: OpenGL_Item(OpenGL_Item_Type::Frame_Buffer, glGenFramebuffer()) {
		}

		inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER, id_); }

		void clear(fvec3 const& color);
		void attach_texture(GLenum attachment, OpenGL_Texture const& texture);
	};
}