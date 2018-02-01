#pragma once

#include <GLFW\glfw3.h>
#include <bitset>

namespace spruce {

	class Input {
	private:
		static std::bitset <GLFW_KEY_LAST> sCurrentKeysDown;
		static std::bitset <GLFW_KEY_LAST> sPrevKeysDown;

		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS) {
				sCurrentKeysDown[key] = true;
			}
			else if(action == GLFW_RELEASE) {
				sCurrentKeysDown[key] = false;
			}
		}

	public:
		static void initialize(GLFWwindow* window) {
			glfwSetKeyCallback(window, &keyCallback);
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

		static void update() {
			for (int i = 0; i < GLFW_KEY_LAST; ++i) {
				sPrevKeysDown[i] = sCurrentKeysDown[i];
			}
		}
	};
}