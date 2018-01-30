#pragma once

#include "spruce_opengl_window.h"
#include "spruce_entity.h"
#include "spruce_opengl_program.h"
#include "spruce_opengl_texture.h"
#include "spruce_opengl_frame_buffer.h"
#include "spruce_mesh.h"
#include "spruce_scene.h"
#include "spruce_renderer.h"

namespace spruce {
	class Game {
	private:
		bool mRunning;

		std::unordered_map<std::string, std::unique_ptr<Scene>> mScenes;
		std::string mActiveSceneName;

		std::unique_ptr<GLWindow> mWindow;
		std::unique_ptr<Renderer> mRenderer;

		bool initialize();
		void update(float dt);
		void draw();

	public:
		inline void setWindow(std::unique_ptr<GLWindow> window) {
			mWindow = std::move(window);
		}

		inline void setRenderer(std::unique_ptr<Renderer> renderer) {
			mRenderer = std::move(renderer);
		}

		Scene& addScene(std::string name, std::unique_ptr<Scene> scene) {
			mScenes[name] = std::move(scene);
			return *mScenes[name].get();
		}

		inline void setActiveSceneName(std::string name) {
			mActiveSceneName = name;
			panic_if(!getActiveScene().initialize(), "Failed to initialize scene '" + name + "'.");
		}

		inline Scene& getActiveScene() { 
			return *mScenes[mActiveSceneName].get(); 
		}

		inline GLWindow& getWindow() { 
			return *mWindow.get(); 
		}

		void run(std::string startSceneName);
	};
}