#pragma once

#include "spruce_graphics.h"
#include <glm/gtc/type_ptr.hpp>

namespace spruce {
	enum class OpenGL_Uniform_Type {
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

	class OpenGL_Uniform {
	private:
		GLuint programId_;
		GLuint location_;
		std::string name_;
		OpenGL_Uniform_Type type_;

		union Variadic_Value {
			float    float_;
			double   double_;
			int32_t  int_;
			uint32_t uint_;
			fvec2    fvec2_;
			fvec3    fvec3_;
			fvec4    fvec4_;
			fmat2x2  fmat2x2_;
			fmat3x3  fmat3x3_;
			fmat4x4  fmat4x4_;

			Variadic_Value() : fmat4x4_() { }
		} value_;

	public:
		OpenGL_Uniform(GLuint programId, std::string const& name) 
		: programId_(programId),
		  location_(glGetUniformLocation(programId, name.c_str())),
		  name_(name) {
		}

		inline GLuint programId() const { return programId_; }
		inline GLuint location() const { return location_; }
		inline std::string const& name() const { return name_; }

		inline float float_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Float, "Invalid uniform access.");
			return value_.float_;
		}

		inline float double_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Double, "Invalid uniform access.");
			return value_.double_;
		}

		inline int32_t int_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Int, "Invalid uniform access.");
			return value_.int_;
		}

		inline uint32_t uint_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Uint, "Invalid uniform access.");
			return value_.uint_;
		}

		inline fvec2 const& fvec2_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Fvec2, "Invalid uniform access.");
			return value_.fvec2_;
		}

		inline fvec3 const& fvec3_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Fvec3, "Invalid uniform access.");
			return value_.fvec3_;
		}

		inline fvec4 const& fvec4_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Fvec4, "Invalid uniform access.");
			return value_.fvec4_;
		}

		inline fmat2x2 const& fmat2x2_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Fmat2x2, "Invalid uniform access.");
			return value_.fmat2x2_;
		}

		inline fmat3x3 const& fmat3x3_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Fmat3x3, "Invalid uniform access.");
			return value_.fmat3x3_;
		}

		inline fmat4x4 const& fmat4x4_value() const {
			panic_if(type_ != OpenGL_Uniform_Type::Fmat4x4, "Invalid uniform access.");
			return value_.fmat4x4_;
		}

		inline void store(float value) {
			type_ = OpenGL_Uniform_Type::Float;
			value_.float_ = value;
			glProgramUniform1f(programId_, location_, value);
		}

		inline void store(double value) {
			type_ = OpenGL_Uniform_Type::Double;
			value_.double_ = value;
			glProgramUniform1d(programId_, location_, value);
		}

		inline void store(int32_t value) {
			type_ = OpenGL_Uniform_Type::Int;
			value_.int_ = value;
			glProgramUniform1i(programId_, location_, value);
		}

		inline void store(uint32_t value) {
			type_ = OpenGL_Uniform_Type::Uint;
			value_.uint_ = value;
			glProgramUniform1ui(programId_, location_, value);
		}

		inline void store(fvec2 const& value) {
			type_ = OpenGL_Uniform_Type::Fvec2;
			value_.fvec2_ = value;
			glProgramUniform2fv(programId_, location_, 1, glm::value_ptr(value));
		}

		inline void store(fvec3 const& value) {
			type_ = OpenGL_Uniform_Type::Fvec3;
			value_.fvec3_ = value;
			glProgramUniform3fv(programId_, location_, 1, glm::value_ptr(value));
		}

		inline void store(fvec4 const& value) {
			type_ = OpenGL_Uniform_Type::Fvec4;
			value_.fvec4_ = value;
			glProgramUniform4fv(programId_, location_, 1, glm::value_ptr(value));
		}

		inline void store(fmat2x2 const& value) {
			type_ = OpenGL_Uniform_Type::Fmat2x2;
			value_.fmat2x2_ = value;
			glProgramUniformMatrix2fv(programId_, location_, 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void store(fmat3x3 const& value) {
			type_ = OpenGL_Uniform_Type::Fmat3x3;
			value_.fmat3x3_ = value;
			glProgramUniformMatrix3fv(programId_, location_, 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void store(fmat4x4 const& value) {
			type_ = OpenGL_Uniform_Type::Fmat4x4;
			value_.fmat4x4_ = value;
			glProgramUniformMatrix4fv(programId_, location_, 1, GL_FALSE, glm::value_ptr(value));
		}
	};
}