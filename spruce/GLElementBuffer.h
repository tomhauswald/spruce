#pragma once

#include "GLBuffer.h"

namespace spruce {

	class GLElementBuffer : public GLBuffer {
	public:
		GLElementBuffer(GLBufferUsage usage)
			: GLBuffer(GLBufferType::Element, usage) {
		}
	};
}