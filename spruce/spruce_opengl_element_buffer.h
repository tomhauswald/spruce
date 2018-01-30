#pragma once

#include "spruce_opengl_buffer.h"

namespace spruce {

	class GLElementBuffer : public GLBuffer {
	public:
		GLElementBuffer(GLBufferUsage usage)
			: GLBuffer(GLBufferType::Element, usage) {
		}
	};
}