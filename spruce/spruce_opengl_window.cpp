#include "spruce_opengl_window.h"
#include "spruce_log.h"

namespace spruce {
	OpenGL_Window::OpenGL_Window(OpenGL_Window_Settings const& settings, OpenGL_Context_Settings const& openglSettings)
		: settings_(settings), openglSettings_(openglSettings) {

		auto glfwInitResult = glfwInit();
		if (glfwInitResult == GLFW_FALSE) {
			panic("Failed to initialize GLFW.");
		}
		else {
			Log::msg.printf(mprintf("Initialized GLFW (Version: %s).\n", glfwGetVersionString()));
		}

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
				Log::msg << "OpenGL core profile not supported with OpenGL version " << openglSettings.majorVersion << "." << openglSettings.minorVersion << ".\n";
				Log::msg << "Falling back to compatibility profile.\n";
			}

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_PROFILE, openglSettings.coreProfileEnabled ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
		}
		
		// Forward compatibility only allowed for OpenGL >= 3.0.
		if (openglSettings.majorVersion < 3) {
			if (openglSettings.forwardCompatibilityEnabled) {
				Log::msg << "Forward compatibility not supported with OpenGL version " << openglSettings.majorVersion << "." << openglSettings.minorVersion << ".\n";
			}

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, openglSettings.forwardCompatibilityEnabled);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglSettings.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglSettings.minorVersion);

#ifdef DEBUG
		// Enable context debug flag.
		glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		window_ = glfwCreateWindow(
			settings.width, 
			settings.height,
			settings.caption.c_str(),
			settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
			nullptr
		);

		if (window_ == nullptr) {
			panic("Failed to create GLFW window.");
		}
		else {
			Log::msg.printf("Created GLFW window of size %dx%d.\n", settings.width, settings.height);
			glfwMakeContextCurrent(window_);
			Log::msg.printf("OpenGL context version: %s\n", glGetString(GL_VERSION));
		}

		

		// Initialize GLEW.
		glewExperimental = GL_TRUE;
		auto glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			glfwTerminate();
			panic(mprintf("Failed to initialize GLEW.\nReason: %s.\n", glewGetErrorString(glewInitResult)));
		}
		else {
			Log::msg.printf("Initialized GLEW (Version: %s).\n", glewGetString(GLEW_VERSION));
		}
	}

	OpenGL_Window::~OpenGL_Window() {
		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	bool OpenGL_Window::should_close() const {
		return glfwWindowShouldClose(window_);
	}

	void OpenGL_Window::swap_buffers() {
		glfwSwapBuffers(window_);
	}

	void OpenGL_Window::poll_events() {
		glfwPollEvents();
	}
}