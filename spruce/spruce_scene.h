#pragma once

#include "spruce_game_object.h"

namespace spruce {
	class Game;
	class Scene {
	private:
		std::unique_ptr<Game_Object> root_;
		Game* game_;

	public:
		Scene() {
			root_ = std::make_unique<Game_Object>();
			root_->set_scene(this);
		}

		virtual bool initialize() { return root_->initialize(); }

		inline Game_Object* root() { return root_.get(); }

		inline Game* game() { return game_; }

		inline void set_game(Game* game) { game_ = game; }
	};
}