#pragma once

#include "spruce_opengl_buffer.h"

namespace spruce {
	class Array_Buffer : public OpenGL_Buffer {
	public:
		Array_Buffer(OpenGL_Buffer_Usage usage)
			: OpenGL_Buffer(OpenGL_Buffer_Type::Array, usage) {
		}
	};
}