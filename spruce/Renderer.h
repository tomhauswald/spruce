#pragma once

#include "Graphics.h"
#include "GLWindow.h"

namespace spruce {

	class Scene;
	class Renderer;

	class RenderPass {
	protected:
		Renderer& mRenderer;
		
	public:
		RenderPass(Renderer& renderer) : 
			mRenderer(renderer) {
		}

		virtual void renderScene(Scene& scene) = 0;
	};

	class Game;

	class Renderer {
	protected:
		std::vector<std::unique_ptr<RenderPass>> mRenderPasses;

	public:
		virtual bool initialize(GLWindow const& window) = 0;

		inline void renderScene(Scene& scene) {
			for (auto& pass : mRenderPasses) {
				pass->renderScene(scene);
			}
		}
	};
}