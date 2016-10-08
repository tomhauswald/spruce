#pragma once

#include "spruce.h"

namespace spruce {

	struct window_settings {
		std::string caption;
		uint width;
		uint height;
		bool doubleBufferingEnabled;
		bool resizable;
		bool maximized;
		bool fullscreen;
	};

	struct opengl_settings {
		uint majorVersion;
		uint minorVersion;
		bool coreProfileEnabled;
		bool forwardCompatibilityEnabled;
	};

	class opengl_window {
	private:
		GLFWwindow* window_;
		window_settings settings_;
		opengl_settings openglSettings_;

	public:
		opengl_window(window_settings const& settings, opengl_settings const& openglSettings);
		~opengl_window();

		inline auto const& settings() const { return settings_; }

		bool should_close() const;
		void swap_buffers();
		void poll_events();
	};
}