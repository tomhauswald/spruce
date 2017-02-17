#pragma once

#include "spruce_graphics.h"

namespace spruce {
	class Scene;
	class Renderer;

	class Render_Pass {
	protected:
		Renderer* renderer_;
		
	public:
		Render_Pass(Renderer* renderer) : renderer_(renderer) {}
		virtual void render(Scene* scene) = 0;
	};

	class Game;
	class Renderer {
	protected:
		Game* game_;
		std::vector<std::unique_ptr<Render_Pass>> passes_;

	public:
		Renderer(Game* game) : game_(game) { }

		virtual bool initialize() = 0;

		inline void render(Scene* scene) {
			for (auto& pass : passes_) {
				pass->render(scene);
			}
		}
	};
}