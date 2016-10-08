#include "spruce_opengl_window.h"
#include "spruce_log.h"

namespace spruce {
	opengl_window::opengl_window(window_settings const& settings, opengl_settings const& openglSettings)
		: settings_(settings), openglSettings_(openglSettings) {

		panic_if(glfwInit() == GLFW_FALSE, "Failed to initialize GLFW.");
		panic_if(glewInit() == GL_FALSE, "Failed to initialize GLEW.");

		int monitorCount;
		auto monitors = glfwGetMonitors(&monitorCount);
		bool useDefaultMonitor = false;

		panic_if(monitorCount <= 0, "No display devices were found.");
		
		// Specify window creation hints.
		glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);
		glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
		glfwWindowHint(GLFW_DOUBLEBUFFER, settings.doubleBufferingEnabled);

		// Core profile only allowed for OpenGL >= 3.2.
		if (openglSettings.majorVersion < 3 || (openglSettings.majorVersion == 3 && openglSettings.minorVersion < 2)) {
			if (openglSettings.coreProfileEnabled) {
				log::msg << "OpenGL core profile not supported with OpenGL version " << openglSettings.majorVersion << "." << openglSettings.minorVersion << "." << newl;
				log::msg << "Falling back to compatibility profile." << newl;
			}

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_PROFILE, openglSettings.coreProfileEnabled ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
		}
		
		// Forward compatibility only allowed for OpenGL >= 3.0.
		if (openglSettings.majorVersion < 3) {
			if (openglSettings.forwardCompatibilityEnabled) {
				log::msg << "Forward compatibility not supported with OpenGL version " << openglSettings.majorVersion << "." << openglSettings.minorVersion << "." << newl;
			}

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, openglSettings.forwardCompatibilityEnabled);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglSettings.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglSettings.minorVersion);

		window_ = glfwCreateWindow(
			settings.width, 
			settings.height,
			settings.caption.c_str(),
			settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
			nullptr
		);

		panic_if(window_ == nullptr, "Failed to create GLFW window.");
	}

	opengl_window::~opengl_window() {
		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	bool opengl_window::should_close() const {
		return glfwWindowShouldClose(window_);
	}

	void opengl_window::swap_buffers() {
		glfwSwapBuffers(window_);
	}

	void opengl_window::poll_events() {
		glfwPollEvents();
	}
}