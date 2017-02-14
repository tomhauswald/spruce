#pragma once

#include "spruce_opengl_item.h"
#include "spruce_opengl_uniform.h"
#include "spruce_opengl_shader.h"
#include "spruce_log.h"

#include <unordered_map>

namespace spruce {
	class OpenGL_Program : public OpenGL_Item {
	private:
		std::string name_;
		std::unordered_map<std::string, std::unique_ptr<OpenGL_Uniform>> uniforms_;

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

		inline OpenGL_Uniform& add_uniform(std::string const& name) {

			// Remember previous program.
			GLuint prevProgramId;
			glGetIntegerv(GL_PROGRAM, (GLint *) &prevProgramId);
			
			// Use this program.
			if (prevProgramId != id_) { glUseProgram(id_); }

			auto& added_uniform = *(uniforms_[name] = std::make_unique<OpenGL_Uniform>(id_, name)).get();

			// Restore previous program.
			if (prevProgramId != id_) { glUseProgram(prevProgramId); }

			return added_uniform;
		}

		inline OpenGL_Uniform* uniform(std::string const& name) {
			return uniforms_.count(name) > 0 ? uniforms_.at(name).get() : nullptr;
		}

		inline OpenGL_Uniform const* uniform(std::string const& name) const {
			return uniforms_.count(name) > 0 ? uniforms_.at(name).get() : nullptr;
		}

		inline void use() { glUseProgram(id_); }

		inline std::string const& name() const { return name_; }

		~OpenGL_Program() {
			glDeleteProgram(id_);
		}
	};
}