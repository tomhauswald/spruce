#pragma once

#include "GLItem.h"
#include <fstream>

namespace spruce {

	enum class GLShaderType {
		Vertex,
		Fragment,
		TesselationControl,
		TesselationEvaluation,
		Geometry
	};

	class GLShader : public GLItem {
	private:
		GLShaderType mShaderType;
		GLenum mGLShaderType;

	public:
		GLShader(GLShaderType type) : 
			GLItem(GLItemType::Shader),
			mShaderType(type) {
			
			switch (type) {
				case GLShaderType::Vertex:
				mGLShaderType = GL_VERTEX_SHADER;
				break;

				case GLShaderType::Fragment:
				mGLShaderType = GL_FRAGMENT_SHADER;
				break;

				case GLShaderType::TesselationControl:
				mGLShaderType = GL_TESS_CONTROL_SHADER;
				break;

				case GLShaderType::TesselationEvaluation:
				mGLShaderType = GL_TESS_EVALUATION_SHADER;
				break;

				case GLShaderType::Geometry:
				mGLShaderType = GL_GEOMETRY_SHADER;
				break;
			}

			mGLId = glCreateShader(mGLShaderType);
		}

		inline GLShaderType getShaderType() const { return mShaderType; }

		void compileFromSource(std::string const& filename);

		~GLShader() {
			glDeleteShader(mGLId);
		}
	};
}