#pragma once

#include "spruce_opengl_item.h"
#include <fstream>

namespace spruce {
	enum class OpenGL_Shader_Type {
		Vertex,
		Fragment,
		Tesselation_Control,
		Tesselation_Evaluation,
		Geometry
	};

	class OpenGL_Shader : public OpenGL_Item {
	private:
		OpenGL_Shader_Type shaderType_;
		GLenum glShaderType_;

	public:
		OpenGL_Shader(OpenGL_Shader_Type type)
			: OpenGL_Item(OpenGL_Item_Type::Shader),
			 shaderType_(type) {
			
			switch (type) {
				case OpenGL_Shader_Type::Vertex:
				glShaderType_ = GL_VERTEX_SHADER;
				break;

				case OpenGL_Shader_Type::Fragment:
				glShaderType_ = GL_FRAGMENT_SHADER;
				break;

				case OpenGL_Shader_Type::Tesselation_Control:
				glShaderType_ = GL_TESS_CONTROL_SHADER;
				break;

				case OpenGL_Shader_Type::Tesselation_Evaluation:
				glShaderType_ = GL_TESS_EVALUATION_SHADER;
				break;

				case OpenGL_Shader_Type::Geometry:
				glShaderType_ = GL_GEOMETRY_SHADER;
				break;
			}

			id_ = glCreateShader(glShaderType_);
		}

		inline OpenGL_Shader_Type shaderType() const { return shaderType_; }

		inline void compile_from_source(std::string const& filename) {
			std::ifstream fstream { filename, std::ios::in };
			panic_if(!fstream.is_open(), mprintf("Failed to open source source file: %s.\n", filename));

			std::string source { (std::istreambuf_iterator<char>(fstream)), std::istreambuf_iterator<char>() };
			GLchar const* source_cstr = source.c_str();
			GLint source_len = source.length();

			glShaderSource(id_, 1, &source_cstr, nullptr);

			Log::msg << "Compiling shader: '" << filename << "'.\n";
			glCompileShader(id_);

			int compileResult;
			glGetShaderiv(id_, GL_COMPILE_STATUS, &compileResult);
			Log::msg << "Compilation result: '" << compileResult << "'.\n";

			if (compileResult != GL_TRUE) {
				int messageLen;
				glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &messageLen);
				if (messageLen > 0) {
					auto buffer = new char[messageLen];
					glGetShaderInfoLog(id_, messageLen, nullptr, buffer);
					Log::msg << buffer << "\n";
					delete[] buffer;
				}
			}
		}

		~OpenGL_Shader() {
			glDeleteShader(id_);
		}
	};
}