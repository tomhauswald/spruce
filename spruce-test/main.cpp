#include <spruce.h>
#include <spruce_log.h>
#include <spruce_opengl_window.h>

using namespace spruce;

int main() {
	log::msg.register_callback([](std::string const& message) { cout << message; });
	log::err.register_callback([](std::string const& message) { cerr << message; });

	window_settings settings;
	settings.caption = "Spruce Engine Test Application";
	settings.width = 1280;
	settings.height = 720;
	settings.doubleBufferingEnabled = true;
	settings.fullscreen = false;
	settings.maximized = false;
	settings.resizable = false;
	
	opengl_settings openglSettings;
	openglSettings.majorVersion = 4;
	openglSettings.minorVersion = 5;
	openglSettings.coreProfileEnabled = true;
	openglSettings.forwardCompatibilityEnabled = true;

	opengl_window window { settings, openglSettings };
	
	while (!window.should_close()) {
		window.poll_events();

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		window.swap_buffers();
	}
}