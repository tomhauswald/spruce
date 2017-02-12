#pragma once

#include "spruce_opengl_item.h"

namespace spruce {
	class OpenGL_Program : public OpenGL_Item {
	public:
		OpenGL_Program()
			: OpenGL_Item(OpenGL_Item_Type::Program, glGenProgram()) {
		}

		inline void attach_shader(OpenGL_Shader const& shader) {
			glAttachShader(id_, shader.id());
		}

		inline void detach_shader(OpenGL_Shader const& shader) {
			glDetachShader(id_, shader.id());
		}

		inline void link() {
			Log::msg << "Linking program.\n";
			glLinkProgram(id_);

			int linkResult;
			glGetProgramiv(id_, GL_LINK_STATUS, &linkResult);
			Log::msg << "Linkage result: '" << linkResult << "'.\n";

			if (linkResult != GL_TRUE) {
				int messageLen;
				glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &messageLen);
				if (messageLen > 0) {
					auto buffer = new char[messageLen];
					glGetProgramInfoLog(id_, messageLen, nullptr, buffer);
					Log::msg << buffer << "\n";
					delete[] buffer;
				}
			}
		}

		~OpenGL_Program() {
			glDeleteProgram(id_);
		}
	};
}