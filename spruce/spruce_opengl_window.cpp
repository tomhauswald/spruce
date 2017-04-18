#include "spruce_opengl_window.h"
#include "spruce_log.h"

namespace spruce {
	std::string OpenGL_Window::glfwErrorMsg_;

	OpenGL_Window::OpenGL_Window(OpenGL_Window_Settings const& window_settings, OpenGL_Context_Settings const& context_settings)
		: window_settings_(window_settings), context_settings_(context_settings) {

		auto glfwInitResult = glfwInit();
		if (glfwInitResult == GLFW_FALSE) {
			panic("Failed to initialize GLFW.");
		}
		else {
			Log::msg.printf(mprintf("Initialized GLFW (Version: %s).\n", glfwGetVersionString()));
			glfwSetErrorCallback(&OpenGL_Window::glfw_error_callback);
		}

		int monitorCount;
		auto monitors = glfwGetMonitors(&monitorCount);
		bool useDefaultMonitor = false;

		panic_if(monitorCount <= 0, "No display devices were found.");
		
		// Specify window creation hints.
		glfwWindowHint(GLFW_MAXIMIZED, window_settings.maximized);
		glfwWindowHint(GLFW_RESIZABLE, window_settings.resizable);
		glfwWindowHint(GLFW_DOUBLEBUFFER, window_settings.doubleBuffered);

		// Core profile only allowed for OpenGL >= 3.2.
		if (context_settings.majorVersion < 3 || (context_settings.majorVersion == 3 && context_settings.minorVersion < 2)) {
			if (context_settings.coreProfileEnabled) {
				Log::msg << "OpenGL core profile not supported with OpenGL version " << context_settings.majorVersion << "." << context_settings.minorVersion << ".\n";
				Log::msg << "Falling back to compatibility profile.\n";
			}

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_PROFILE, context_settings.coreProfileEnabled ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
		}
		
		// Forward compatibility only allowed for OpenGL >= 3.0.
		if (context_settings.majorVersion < 3) {
			if (context_settings.forwardCompatibilityEnabled) {
				Log::msg << "Forward compatibility not supported with OpenGL version " << context_settings.majorVersion << "." << context_settings.minorVersion << ".\n";
			}

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, context_settings.forwardCompatibilityEnabled);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, context_settings.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, context_settings.minorVersion);

#ifdef _DEBUG
		// Enable context debug flag.
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		window_ = glfwCreateWindow(
			window_settings.width, 
			window_settings.height,
			window_settings.caption.c_str(),
			window_settings.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
			nullptr
		);

		if (window_ == nullptr) {
			panic(mprintf("Failed to create GLFW window.\nReason: %s.\n", OpenGL_Window::glfwErrorMsg_.c_str()));
		}
		else {
			Log::msg.printf("Created GLFW window of size %dx%d.\n", window_settings.width, window_settings.height);
			glfwMakeContextCurrent(window_);
			if (window_settings.vsync) {
				glfwSwapInterval(1);
			}
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

	void OpenGL_Window::clear_buffer(fvec3 const& color) {
		glClearColor(color.r, color.g, color.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}