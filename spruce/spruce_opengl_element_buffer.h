#pragma once

#include "spruce_opengl_buffer.h"

namespace spruce {
	class OpenGL_Element_Buffer : public OpenGL_Buffer {
	public:
		OpenGL_Element_Buffer(OpenGL_Buffer_Usage usage)
			: OpenGL_Buffer(OpenGL_Buffer_Type::Element, usage) {
		}
	};
}