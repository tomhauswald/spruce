#include "Input.h"

namespace spruce {

	std::bitset<GLFW_KEY_LAST> Input::sCurrentKeysDown;
	std::bitset<GLFW_KEY_LAST> Input::sPrevKeysDown;
	glm::dvec2 Input::sCurrentCursorPos;
	glm::dvec2 Input::sPrevCursorPos;
}