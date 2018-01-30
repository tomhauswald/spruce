#pragma once

#include "spruce_opengl_item.h"

namespace spruce {

	enum class GLBufferUsage {
		StaticRead,
		StaticCopy,
		StaticDraw,

		DynamicRead,
		DynamicCopy,
		DynamicDraw,

		StreamRead,
		StreamCopy,
		StreamDraw
	};

	enum class GLBufferType {
		Array,
		Element
	};

	class GLBuffer : public GLItem {
	private:
		GLBufferType mBufferType;
		GLenum mGLBufferTarget;

		GLBufferUsage mBufferUsage;
		GLenum mGLBufferUsage;

	public:
		GLBuffer(GLBufferType type, GLBufferUsage usage)
			: GLItem(GLItemType::Buffer, glGenBuffer()),
			  mBufferType(type),
			  mBufferUsage(usage) {
			
			// Map to OpenGL buffer target.
			switch (mBufferType) {
				case GLBufferType::Array:
				mGLBufferTarget = GL_ARRAY_BUFFER;
				break;

				case GLBufferType::Element:
				mGLBufferTarget = GL_ELEMENT_ARRAY_BUFFER;
				break;
			}

			// Map to OpenGL buffer usage.
			switch (mBufferUsage) {
				case GLBufferUsage::StaticRead:
				mGLBufferUsage = GL_STATIC_READ;
				break;

				case GLBufferUsage::StaticCopy:
				mGLBufferUsage = GL_STATIC_COPY;
				break;

				case GLBufferUsage::StaticDraw:
				mGLBufferUsage = GL_STATIC_DRAW;
				break;

				case GLBufferUsage::DynamicRead:
				mGLBufferUsage = GL_DYNAMIC_READ;
				break;

				case GLBufferUsage::DynamicCopy:
				mGLBufferUsage = GL_DYNAMIC_COPY;
				break;

				case GLBufferUsage::DynamicDraw:
				mGLBufferUsage = GL_DYNAMIC_DRAW;
				break;

				case GLBufferUsage::StreamRead:
				mGLBufferUsage = GL_STREAM_READ;
				break;

				case GLBufferUsage::StreamCopy:
				mGLBufferUsage = GL_STREAM_COPY;
				break;

				case GLBufferUsage::StreamDraw:
				mGLBufferUsage = GL_STREAM_DRAW;
				break;
			}
		}

		~GLBuffer() {
			glDeleteBuffer(mGLId);
		}

		void bind() {
			glBindBuffer(mGLBufferTarget, mGLId);
		}

		void buffer(uint32_t sizeInBytes, void const* data) {
			glBufferData(mGLBufferTarget, static_cast<GLsizeiptr>(sizeInBytes), data, mGLBufferUsage);
		}

		inline GLBufferType getBufferType() const { return mBufferType; }
	};
}