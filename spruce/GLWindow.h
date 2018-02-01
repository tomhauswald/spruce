#pragma once

#include "Graphics.h"
#include "Input.h"

namespace spruce {

	struct GLWindowSettings {
		std::string caption;
		uint16_t width;
		uint16_t height;
		bool doubleBuffered;
		bool vsync;
		bool resizable;
		bool maximized;
		bool fullscreen;
	};

	struct GLContextSettings {
		uint8_t majorVersion;
		uint8_t minorVersion;
		bool coreProfileEnabled;
		bool forwardCompatibilityEnabled;
	};

	class GLWindow {
	private:
		GLFWwindow* mGLFWWindow;
		GLWindowSettings mWindowSettings;
		GLContextSettings mContextSettings;
		
		static std::string sGLFWErrorMsg;

		static inline void glfwErrorCallback(int code, char const* desc) {
			sGLFWErrorMsg = std::string { desc };
		}

	public:
		GLWindow(GLWindowSettings windowSettings, GLContextSettings contextSettings);

		inline GLWindowSettings const& getWindowSettings() const { return mWindowSettings; }
		inline GLContextSettings const& getContextSettings() const { return mContextSettings; }
		
		inline uint16_t getWidth() const { return mWindowSettings.width; }
		inline uint16_t getHeight() const { return mWindowSettings.height; }
		inline float getAspectRatio() const { return getWidth() / (float)getHeight(); }

		bool isCloseRequested() const;
		void swapBuffers();
		void pollEvents();
		void clear(fvec3 const& color);

		~GLWindow();
	};
}