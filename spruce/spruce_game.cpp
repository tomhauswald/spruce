#include "spruce_game.h"

namespace spruce {

	bool Game::initialize() {
		panic_if(window_ == nullptr,   "Window must be specified.");
		panic_if(renderer_ == nullptr, "Renderer must be specified.");
		return true;
	}

	void Game::update(float dt) {
		window_->poll_events();
		active_scene()->root()->update(dt);
	}

	void Game::draw() {
		window_->clear_buffer({ 0.0f, 0.0f, 0.0f });

		if (renderer_) {
			renderer_->render(active_scene());
		}

		window_->swap_buffers();
	}

	void Game::run() {
		static double current_time = glfwGetTime();
		static double last_frame_time = glfwGetTime();

		panic_if(!initialize(), "Game initialization failed.");
		panic_if(!active_scene()->initialize(), "Initialization of start scene failed.");
		running_ = true;

		while (running_) {
			current_time = glfwGetTime();
			auto dt = static_cast<float>(glfwGetTime() - last_frame_time);
			last_frame_time = current_time;

			update(glfwGetTime() - last_frame_time);
			draw();

			running_ &= !window_->should_close();
		}
	}
}