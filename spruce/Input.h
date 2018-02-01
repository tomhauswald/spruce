#pragma once

#include "Graphics.h"
#include <bitset>

namespace spruce {

	class Input {
	private:
		static std::bitset<GLFW_KEY_LAST> sCurrentKeysDown;
		static std::bitset<GLFW_KEY_LAST> sPrevKeysDown;
		static glm::dvec2 sCurrentCursorPos;
		static glm::dvec2 sPrevCursorPos;

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS) {
				sCurrentKeysDown[key] = true;
			}
			else if(action == GLFW_RELEASE) {
				sCurrentKeysDown[key] = false;
			}
		}

		static void cursorCallback(GLFWwindow* window, double x, double y) {
			sCurrentCursorPos.x = x;
			sCurrentCursorPos.y = y;
		}

	public:
		static void initialize(GLFWwindow* window) {
			glfwSetKeyCallback(window, &keyCallback);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, &cursorCallback);
		}

		static bool isKeyDown(int key) {
			return sCurrentKeysDown[key];
		}

		static bool wasKeyPressed(int key) {
			return sCurrentKeysDown[key] && !sPrevKeysDown[key];
		}

		static bool wasKeyReleased(int key) {
			return !sCurrentKeysDown[key] && sPrevKeysDown[key];
		}

		static glm::dvec2 const& getCursorPosition() {
			return sCurrentCursorPos;
		}

		static glm::dvec2 getCursorDelta() {
			return sCurrentCursorPos - sPrevCursorPos;
		}

		static void update() {
			for (int i = 0; i < GLFW_KEY_LAST; ++i) {
				sPrevKeysDown[i] = sCurrentKeysDown[i];
			}
			sPrevCursorPos = sCurrentCursorPos;
		}
	};
}