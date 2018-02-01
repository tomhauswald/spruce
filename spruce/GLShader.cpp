#include "GLShader.h"
#include "Common.h"
#include "Log.h"

namespace spruce {

	void GLShader::compileFromSource(std::string const& filename) {
		std::ifstream fstream { filename, std::ios::in };
		panicIf(!fstream.is_open(), "Failed to open source source file: '" + filename + "'.\n");

		std::string source { (std::istreambuf_iterator<char>(fstream)), std::istreambuf_iterator<char>() };
		GLchar const* source_cstr = source.c_str();
		GLint source_len = source.length();

		glShaderSource(mGLId, 1, &source_cstr, nullptr);
		glCompileShader(mGLId);

		int compileResult;
		glGetShaderiv(mGLId, GL_COMPILE_STATUS, &compileResult);

		if (compileResult != GL_TRUE) {
			Log::err << "Failed to compile shader '" << filename << "'. Reason:\n";
			int messageLen;
			glGetShaderiv(mGLId, GL_INFO_LOG_LENGTH, &messageLen);
			if (messageLen > 0) {
				auto buffer = std::make_unique<char[]>(messageLen);
				glGetShaderInfoLog(mGLId, messageLen, nullptr, buffer.get());
				Log::msg << buffer.get() << "\n";
			}
		}
		else {
			Log::msg << "Successfully compiled shader '" << filename << "'.\n";
		}
	}
}