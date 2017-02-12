#pragma once

#include "spruce_common.h"

#define GLEW_STATIC
#define GLFW_STATIC

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

namespace spruce {
	using namespace glm;

	using glGenFunc    = void (__stdcall *) (GLsizei, GLuint *);
	using glDeleteFunc = void (__stdcall *) (GLsizei, GLuint const*);

	// Generates a single OpenGL resource using the specified glGen(...) function.
	static inline GLuint glGenSingle(glGenFunc genFunc) {
		GLuint id;
		genFunc(1, &id);
		return id;
	}

	// Deletes the OpenGL resource with the given id, using the specified glDelete(...) function.
	static inline void glDeleteSingle(glDeleteFunc deleteFunc, GLuint id) {
		deleteFunc(1, &id);
	}

	static inline GLuint glGenTexture()      { return glGenSingle(glGenTextures); }
	static inline GLuint glGenBuffer()       { return glGenSingle(glGenBuffers); }
	static inline GLuint glGenFramebuffer()  { return glGenSingle(glGenFramebuffers); }
	static inline GLuint glGenProgram()      { return glGenSingle(glGenProgramsARB); }
	static inline GLuint glGenVertexArray()  { return glGenSingle(glGenVertexArrays); }

	static inline void glDeleteTexture(GLuint id)     { glDeleteSingle(glDeleteTextures, id); }
	static inline void glDeleteBuffer(GLuint id)      { glDeleteSingle(glDeleteBuffers, id); }
	static inline void glDeleteFramebuffer(GLuint id) { glDeleteSingle(glDeleteFramebuffers, id); }
	static inline void glDeleteVertexArray(GLuint id) { glDeleteSingle(glDeleteVertexArrays, id); }
}