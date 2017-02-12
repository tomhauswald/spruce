#pragma once

#include "spruce_opengl_item.h"

namespace spruce {
	class OpenGL_Program : public OpenGL_Item {
	private:
		std::string name_;

	public:
		OpenGL_Program(std::string const& name)
			: OpenGL_Item(OpenGL_Item_Type::Program, glCreateProgram()),
			  name_(name) {
		}

		inline void attach_shader(OpenGL_Shader const& shader) {
			glAttachShader(id_, shader.id());
		}

		inline void detach_shader(OpenGL_Shader const& shader) {
			glDetachShader(id_, shader.id());
		}

		inline void link() {
			glLinkProgram(id_);

			int linkResult;
			glGetProgramiv(id_, GL_LINK_STATUS, &linkResult);

			if (linkResult != GL_TRUE) {
				Log::err << "Failed to link program '" << name_ << "'. Reason:\n";
				int messageLen;
				glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &messageLen);
				if (messageLen > 0) {
					auto buffer = std::make_unique<char[]>(messageLen);
					glGetProgramInfoLog(id_, messageLen, nullptr, buffer.get());
					Log::msg << buffer.get() << "\n";
				}
			}
			else {
				Log::msg << "Successfully linked program '" << name_ << "'.\n";
			}
		}

		inline std::string const& name() const { return name_; }

		~OpenGL_Program() {
			glDeleteProgram(id_);
		}
	};
}