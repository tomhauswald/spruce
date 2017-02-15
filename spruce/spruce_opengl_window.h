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
		OpenGL_Window_Settings window_settings_;
		OpenGL_Context_Settings context_settings_;
		
		static std::string glfwErrorMsg_;

		static inline void glfw_error_callback(int code, char const* desc) {
			glfwErrorMsg_ = std::string { desc };
		}

	public:
		OpenGL_Window(OpenGL_Window_Settings const& window_settings, OpenGL_Context_Settings const& context_settings);
		~OpenGL_Window();

		inline OpenGL_Window_Settings const& window_settings() const { return window_settings_; }
		inline OpenGL_Context_Settings const& context_settings() const { return context_settings_; }
		inline float aspect_ratio() const { return window_settings_.width / static_cast<float>(window_settings_.height); }
		
		bool should_close() const;
		void swap_buffers();
		void poll_events();
		void clear_buffer(fvec3 const& color);
	};
}