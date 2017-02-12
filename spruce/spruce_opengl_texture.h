#pragma once

#include "spruce_opengl_item.h"

namespace spruce {
	class OpenGL_Texture : public OpenGL_Item {
	public:
		OpenGL_Texture()
			: OpenGL_Item(OpenGL_Item_Type::Texture, glGenTexture()) {
			panic_if(id_ == 0, "Failed to generate OpenGL texture.");
		}

		~OpenGL_Texture() {
			glDeleteTexture(id_);
		}
	};
}