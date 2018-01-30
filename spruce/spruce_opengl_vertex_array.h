#pragma once

#include "spruce_graphics.h"
#include "spruce_opengl_item.h"

namespace spruce {

	class GLVertexArray : public GLItem {
	public:
		GLVertexArray()
			: GLItem(GLItemType::VertexArray, glGenVertexArray()) {}

		inline void bind() {
			glBindVertexArray(mGLId);
		}

		inline void enableAttributeSlot(GLuint slot) {
			glEnableVertexAttribArray(slot);
		}

		/*
		 * Store float vector attributes.
		 */

		inline void storeFvecAttribute(GLuint slot, size_t vectorLen, size_t stride = 0, size_t offset = 0) {
			glVertexAttribPointer(slot, vectorLen, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void const*)offset);
		}

		inline void storeFloatAttribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			storeFvecAttribute(slot, 1, stride, offset);
		}

		inline void storeFvec2Attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			storeFvecAttribute(slot, 2, stride, offset);
		}

		inline void storeFvec3Attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			storeFvecAttribute(slot, 3, stride, offset);
		}

		inline void storeFvec4Attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			storeFvecAttribute(slot, 4, stride, offset);
		}

		~GLVertexArray() {
			glDeleteVertexArray(mGLId);
		}
	};
}