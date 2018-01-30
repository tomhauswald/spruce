#include "spruce_game.h"

namespace spruce {

	bool Game::initialize() {
		panic_if(mWindow == nullptr,   "Window must be specified.");
		panic_if(mRenderer == nullptr, "Renderer must be specified.");
		return true;
	}

	void Game::update(float dt) {
		mWindow->pollEvents();
		getActiveScene().getRoot().update(dt);
	}

	void Game::draw() {
		mWindow->clear({ 0.0f, 0.0f, 0.0f });
		mRenderer->renderScene(getActiveScene());
		mWindow->swapBuffers();
	}

	void Game::run(std::string startSceneName) {
		static double currentTime = glfwGetTime();
		static double lastFrameTime = glfwGetTime();

		mActiveSceneName = startSceneName;
		panic_if(!initialize(), "Game initialization failed.");
		panic_if(!mScenes[startSceneName]->initialize(), "Initialization of start scene failed.");
		mRunning = true;

		while (mRunning) {
			lastFrameTime = currentTime;
			currentTime = glfwGetTime();
			auto dt = static_cast<float>(glfwGetTime() - lastFrameTime);

			update(dt);
			draw();

			mRunning &= !mWindow->isCloseRequested();
		}
	}
}