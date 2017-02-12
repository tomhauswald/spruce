#pragma once

#include "spruce_graphics.h"

namespace spruce {
	enum class OpenGL_Item_Type {
		Texture,
		Buffer,
		Framebuffer,
		Shader,
		Program,
		Vertex_Array
	};

	class OpenGL_Item {
	protected:
		OpenGL_Item_Type type_;
		GLuint id_;

	public:
		OpenGL_Item(OpenGL_Item_Type type) 
			: type_(type), id_(0) {
		}

		OpenGL_Item(OpenGL_Item_Type type, GLuint id) 
			: type_(type), id_(id) {
		}

		inline OpenGL_Item_Type type() const { return type_; }
		inline GLuint id() const { return id_; }
	};
}