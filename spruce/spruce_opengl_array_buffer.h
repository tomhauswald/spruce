#pragma once

#include "spruce_opengl_buffer.h"

namespace spruce {
	class OpenGL_Array_Buffer : public OpenGL_Buffer {
	public:
		OpenGL_Array_Buffer(OpenGL_Buffer_Usage usage)
			: OpenGL_Buffer(OpenGL_Buffer_Type::Array, usage) {
		}
	};
}