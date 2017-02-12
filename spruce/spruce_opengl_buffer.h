#pragma once

#include "spruce_opengl_item.h"

namespace spruce {

	enum class OpenGL_Buffer_Usage {
		Static_Read,
		Static_Copy,
		Static_Draw,

		Dynamic_Read,
		Dynamic_Copy,
		Dynamic_Draw,

		Stream_Read,
		Stream_Copy,
		Stream_Draw
	};

	enum class OpenGL_Buffer_Type {
		Array,
		Element
	};

	class OpenGL_Buffer : public OpenGL_Item {
	private:
		OpenGL_Buffer_Type bufferType_;
		GLenum glBufferTarget_;

		OpenGL_Buffer_Usage bufferUsage_;
		GLenum glBufferUsage_;

	public:
		OpenGL_Buffer(OpenGL_Buffer_Type type, OpenGL_Buffer_Usage usage)
			: OpenGL_Item(OpenGL_Item_Type::Buffer, glGenBuffer()),
			  bufferType_(type),
			  bufferUsage_(usage) {
			
			// Map to OpenGL buffer target.
			switch (bufferType_) {
				case OpenGL_Buffer_Type::Array:
				glBufferTarget_ = GL_ARRAY_BUFFER;
				break;

				case OpenGL_Buffer_Type::Element:
				glBufferTarget_ = GL_ELEMENT_ARRAY_BUFFER;
				break;
			}

			// Map to OpenGL buffer usage.
			switch (bufferUsage_) {
				case OpenGL_Buffer_Usage::Static_Read:
				glBufferUsage_ = GL_STATIC_READ;
				break;

				case OpenGL_Buffer_Usage::Static_Copy:
				glBufferUsage_ = GL_STATIC_COPY;
				break;

				case OpenGL_Buffer_Usage::Static_Draw:
				glBufferUsage_ = GL_STATIC_DRAW;
				break;

				case OpenGL_Buffer_Usage::Dynamic_Read:
				glBufferUsage_ = GL_DYNAMIC_READ;
				break;

				case OpenGL_Buffer_Usage::Dynamic_Copy:
				glBufferUsage_ = GL_DYNAMIC_COPY;
				break;

				case OpenGL_Buffer_Usage::Dynamic_Draw:
				glBufferUsage_ = GL_DYNAMIC_DRAW;
				break;

				case OpenGL_Buffer_Usage::Stream_Read:
				glBufferUsage_ = GL_STREAM_READ;
				break;

				case OpenGL_Buffer_Usage::Stream_Copy:
				glBufferUsage_ = GL_STREAM_COPY;
				break;

				case OpenGL_Buffer_Usage::Stream_Draw:
				glBufferUsage_ = GL_STREAM_DRAW;
				break;
			}
		}

		~OpenGL_Buffer() {
			glDeleteBuffer(id_);
		}

		void bind() {
			glBindBuffer(glBufferTarget_, id_);
		}

		void buffer(uint32_t sizeInBytes, void const* data) {
			glBufferData(glBufferTarget_, static_cast<GLsizeiptr>(sizeInBytes), data, glBufferUsage_);
		}

		inline OpenGL_Buffer_Type bufferType() const { return bufferType_; }
	};
}