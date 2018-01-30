#pragma once

#include "spruce_opengl_item.h"
#include "spruce_opengl_uniform.h"
#include "spruce_opengl_shader.h"
#include "spruce_log.h"

#include <unordered_map>

namespace spruce {
	class GLShaderProgram : public GLItem {
	private:
		std::string mName;
		std::unordered_map<std::string, std::unique_ptr<GLUniformVar>> mUniformVars;

	public:
		GLShaderProgram(std::string name)
			: GLItem(GLItemType::ShaderProgram, glCreateProgram()),
			  mName(name) {
		}

		inline void addShader(GLShader& shader) {
			glAttachShader(mGLId, shader.getGLId());
		}

		inline void removeShader(GLShader& shader) {
			glDetachShader(mGLId, shader.getGLId());
		}

		inline void link() {
			glLinkProgram(mGLId);

			int linkResult;
			glGetProgramiv(mGLId, GL_LINK_STATUS, &linkResult);

			if (linkResult != GL_TRUE) {
				Log::err << "Failed to link program '" << mName << "'. Reason:\n";
				int messageLen;
				glGetProgramiv(mGLId, GL_INFO_LOG_LENGTH, &messageLen);
				if (messageLen > 0) {
					auto buffer = std::make_unique<char[]>(messageLen);
					glGetProgramInfoLog(mGLId, messageLen, nullptr, buffer.get());
					Log::msg << buffer.get() << "\n";
				}
			}
			else {
				Log::msg << "Successfully linked program '" << mName << "'.\n";
			}
		}

		inline GLUniformVar& addUniformVar(std::string name) {
			return *(mUniformVars[name] = std::make_unique<GLUniformVar>(mGLId, name)).get();
		}

		inline GLUniformVar& getUniformVar(std::string const& name) {
			return *mUniformVars.at(name).get();
		}

		inline void use() { 
			glUseProgram(mGLId); 
		}

		inline std::string const& name() const { 
			return mName; 
		}

		~GLShaderProgram() {
			glDeleteProgram(mGLId);
		}
	};
}