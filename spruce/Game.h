#pragma once

#include "GLWindow.h"
#include "Entity.h"
#include "GLShaderProgram.h"
#include "GLTexture.h"
#include "GLFramebuffer.h"
#include "Mesh.h"
#include "Scene.h"
#include "Renderer.h"

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
			panicIf(!getActiveScene().initialize(), "Failed to initialize scene '" + name + "'.");
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