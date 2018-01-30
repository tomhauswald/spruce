#pragma once

#include "spruce_graphics.h"

namespace spruce {
	enum class GLItemType {
		Texture,
		Buffer,
		Framebuffer,
		Shader,
		ShaderProgram,
		VertexArray
	};

	class GLItem {
	protected:
		GLItemType mType;
		GLuint mGLId;

	public:
		GLItem(GLItemType type, GLuint glId = 0)
			: mType(type), mGLId(glId) {
		}

		inline GLItemType getType() const { return mType; }
		inline GLuint getGLId() const { return mGLId; }
	};
}