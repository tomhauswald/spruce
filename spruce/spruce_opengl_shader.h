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

		void compile_from_source(std::string const& filename);

		~OpenGL_Shader() {
			glDeleteShader(id_);
		}
	};
}