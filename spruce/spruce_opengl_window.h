#pragma once

#include "spruce_graphics.h"

namespace spruce {

	struct OpenGL_Window_Settings {
		std::string caption;
		uint width;
		uint height;
		bool doubleBufferingEnabled;
		bool resizable;
		bool maximized;
		bool fullscreen;
	};

	struct OpenGL_Context_Settings {
		uint majorVersion;
		uint minorVersion;
		bool coreProfileEnabled;
		bool forwardCompatibilityEnabled;
	};

	class OpenGL_Window {
	private:
		GLFWwindow* window_;
		OpenGL_Window_Settings settings_;
		OpenGL_Context_Settings openglSettings_;

	public:
		OpenGL_Window(OpenGL_Window_Settings const& settings, OpenGL_Context_Settings const& openglSettings);
		~OpenGL_Window();

		inline auto const& settings() const { return settings_; }

		bool should_close() const;
		void swap_buffers();
		void poll_events();
	};
}