#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>

#define GLEW_STATIC
#define GLFW_STATIC

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

namespace spruce {
	using namespace glm;
	
	using vec2 = glm::fvec2;
	using vec3 = glm::fvec3;
	using vec4 = glm::fvec4;

	using std::cout;
	using std::cerr;
	using std::endl;
	using std::cin;

	const auto newl = "\n"; // New-line character.

	void panic_if(bool expression, std::string const& message);
	inline void panic(std::string const& message) { panic_if(true, message); }
}