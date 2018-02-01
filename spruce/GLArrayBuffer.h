#pragma once

#include "GLBuffer.h"

namespace spruce {
	class GLArrayBuffer : public GLBuffer {
	public:
		GLArrayBuffer(GLBufferUsage usage)
			: GLBuffer(GLBufferType::Array, usage) {
		}
	};
}