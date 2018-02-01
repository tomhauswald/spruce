#include "GLWindow.h"
#include "Log.h"

namespace spruce {
	std::string GLWindow::sGLFWErrorMsg;

	GLWindow::GLWindow(GLWindowSettings windowSettings, GLContextSettings contextSettings) : 
		mWindowSettings(windowSettings), 
		mContextSettings(contextSettings) {

		panicIf(glfwInit() == GLFW_FALSE, "Failed to initialize GLFW.");

		Log::msg.printf(mprintf("Initialized GLFW (Version: %s).\n", glfwGetVersionString()));
		glfwSetErrorCallback(&GLWindow::glfwErrorCallback);

		int monitorCount;
		auto monitors = glfwGetMonitors(&monitorCount);
		bool useDefaultMonitor = false;

		panicIf(monitorCount <= 0, "No display devices were found.");
		
		// Specify window creation hints.
		glfwWindowHint(GLFW_MAXIMIZED, windowSettings.maximized);
		glfwWindowHint(GLFW_RESIZABLE, windowSettings.resizable);
		glfwWindowHint(GLFW_DOUBLEBUFFER, windowSettings.doubleBuffered);

		// Core profile only allowed for OpenGL >= 3.2.
		if (contextSettings.majorVersion < 3 || (contextSettings.majorVersion == 3 && contextSettings.minorVersion < 2)) {
			if (contextSettings.coreProfileEnabled) {
				Log::msg << "OpenGL core profile not supported with OpenGL version " << contextSettings.majorVersion << "." << contextSettings.minorVersion << ".\n";
				Log::msg << "Falling back to compatibility profile.\n";
			}

			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_PROFILE, contextSettings.coreProfileEnabled ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
		}
		
		// Forward compatibility only allowed for OpenGL >= 3.0.
		if (contextSettings.majorVersion < 3) {
			if (contextSettings.forwardCompatibilityEnabled) {
				Log::msg << "Forward compatibility not supported with OpenGL version " << contextSettings.majorVersion << "." << contextSettings.minorVersion << ".\n";
			}

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
		}
		else {
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, contextSettings.forwardCompatibilityEnabled);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, contextSettings.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, contextSettings.minorVersion);

#ifdef _DEBUG
		// Enable context debug flag.
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

		mGLFWWindow = glfwCreateWindow(
			windowSettings.width, 
			windowSettings.height,
			windowSettings.caption.c_str(),
			windowSettings.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
			nullptr
		);

		if (mGLFWWindow == nullptr) {
			panic(mprintf("Failed to create GLFW window.\nReason: %s.\n", GLWindow::sGLFWErrorMsg.c_str()));
		}
		else {
			Log::msg.printf("Created GLFW window of size %dx%d.\n", windowSettings.width, windowSettings.height);
			glfwMakeContextCurrent(mGLFWWindow);
			if (windowSettings.vsync) {
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
		
		// Initialize input module.
		Input::initialize(mGLFWWindow);
	}

	GLWindow::~GLWindow() {
		glfwDestroyWindow(mGLFWWindow);
		glfwTerminate();
	}

	bool GLWindow::isCloseRequested() const {
		return glfwWindowShouldClose(mGLFWWindow);
	}

	void GLWindow::swapBuffers() {
		glfwSwapBuffers(mGLFWWindow);
	}

	void GLWindow::pollEvents() {
		glfwPollEvents();
	}

	void GLWindow::clear(fvec3 const& color) {
		glClearColor(color.r, color.g, color.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}