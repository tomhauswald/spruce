#pragma once

#include "spruce_opengl_buffer.h"

namespace spruce {
	class GLArrayBuffer : public GLBuffer {
	public:
		GLArrayBuffer(GLBufferUsage usage)
			: GLBuffer(GLBufferType::Array, usage) {
		}
	};
}