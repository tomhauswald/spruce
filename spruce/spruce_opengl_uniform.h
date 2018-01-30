#pragma once

#include "spruce_graphics.h"
#include <glm/gtc/type_ptr.hpp>
#include <variant>

namespace spruce {

	enum class GLUniformVarType {
		Float,
		Double,
		Int,
		Uint,
		Fvec2,
		Fvec3,
		Fvec4,
		Dvec2,
		Dvec3,
		Dvec4,
		Fmat2x2,
		Fmat3x3,
		Fmat4x4
	};

	class GLUniformVar {
	private:
		GLuint mProgramId;
		GLuint mLocation;
		std::string mName;
		GLUniformVarType mVarType;

		std::variant<
			float, double, 
			int32_t, uint32_t, 
			fvec2, fvec3, fvec4, 
			fmat2x2, fmat3x3, fmat4x4
		> mValue;

	public:
		GLUniformVar(GLuint programId, std::string name) : 
			mProgramId(programId),
			mLocation(glGetUniformLocation(programId, name.c_str())),
			mName(name) {
		}

		inline GLuint getProgramId() const { return mProgramId; }
		inline GLuint getLocation() const { return mLocation; }
		inline std::string const& getName() const { return mName; }
		inline GLUniformVarType getVarType() const { return mVarType; }

		template<typename T>
		T read() {
			return std::get<T>(mValue);
		}

		inline void store(float value) {
			mVarType = GLUniformVarType::Float;
			mValue = value;
			glProgramUniform1f(mProgramId, mLocation, value);
		}

		inline void store(double value) {
			mVarType = GLUniformVarType::Double;
			mValue = value;
			glProgramUniform1d(mProgramId, mLocation, value);
		}

		inline void store(int32_t value) {
			mVarType = GLUniformVarType::Int;
			mValue = value;
			glProgramUniform1i(mProgramId, mLocation, value);
		}

		inline void store(uint32_t value) {
			mVarType = GLUniformVarType::Uint;
			mValue = value;
			glProgramUniform1ui(mProgramId, mLocation, value);
		}

		inline void store(fvec2 value) {
			mVarType = GLUniformVarType::Fvec2;
			mValue = value;
			glProgramUniform2fv(mProgramId, mLocation, 1, glm::value_ptr(value));
		}

		inline void store(fvec3 value) {
			mVarType = GLUniformVarType::Fvec3;
			mValue = value;
			glProgramUniform3fv(mProgramId, mLocation, 1, glm::value_ptr(value));
		}

		inline void store(fvec4 value) {
			mVarType = GLUniformVarType::Fvec4;
			mValue = value;
			glProgramUniform4fv(mProgramId, mLocation, 1, glm::value_ptr(value));
		}

		inline void store(fmat2x2 value) {
			mVarType = GLUniformVarType::Fmat2x2;
			mValue = value;
			glProgramUniformMatrix2fv(mProgramId, mLocation, 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void store(fmat3x3 value) {
			mVarType = GLUniformVarType::Fmat3x3;
			mValue = value;
			glProgramUniformMatrix3fv(mProgramId, mLocation, 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void store(fmat4x4 value) {
			mVarType = GLUniformVarType::Fmat4x4;
			mValue = value;
			glProgramUniformMatrix4fv(mProgramId, mLocation, 1, GL_FALSE, glm::value_ptr(value));
		}
	};
}