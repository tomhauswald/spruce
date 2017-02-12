#pragma once

#include "spruce_graphics.h"
#include "spruce_opengl_item.h"

namespace spruce {
	class OpenGL_Vertex_Array : public OpenGL_Item {
	public:
		OpenGL_Vertex_Array()
			: OpenGL_Item(OpenGL_Item_Type::Vertex_Array, glGenVertexArray()) {}

		inline void bind() {
			glBindVertexArray(id_);
		}

		inline void enable_attribute(GLuint slot) {
			glEnableVertexAttribArray(slot);
		}

		/*
		 * Store float vector attributes.
		 */

		inline void store_float_attribute(GLuint slot, size_t vectorLen, size_t stride = 0, size_t offset = 0) {
			glVertexAttribPointer(slot, vectorLen, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void const*)offset);
		}

		inline void store_float_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_float_attribute(slot, 1, stride, offset);
		}

		inline void store_fvec2_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_float_attribute(slot, 2, stride, offset);
		}

		inline void store_fvec3_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_float_attribute(slot, 3, stride, offset);
		}

		inline void store_fvec4_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_float_attribute(slot, 4, stride, offset);
		}

		/*
		 * Store double vector attributes.
		 */

		inline void store_double_attribute(GLuint slot, size_t vectorLen, size_t stride = 0, size_t offset = 0) {
			glVertexAttribPointer(slot, vectorLen, GL_DOUBLE, GL_FALSE, static_cast<GLsizei>(stride), (void const*)offset);
		}

		inline void store_double_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_double_attribute(slot, 1, stride, offset);
		}

		inline void store_dvec2_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_double_attribute(slot, 2, stride, offset);
		}

		inline void store_dvec3_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_double_attribute(slot, 3, stride, offset);
		}

		inline void store_dvec4_attribute(GLuint slot, size_t stride = 0, size_t offset = 0) {
			store_double_attribute(slot, 4, stride, offset);
		}

		~OpenGL_Vertex_Array() {
			glDeleteVertexArray(id_);
		}
	};
}