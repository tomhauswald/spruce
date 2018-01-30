#pragma once

#include "spruce_entity.h"

namespace spruce {

	class Game;

	class Scene {
	private:
		std::unique_ptr<Entity> mRoot;
		Game& mGame;

	public:
		Scene(Game& game) :
			mGame(game) {
			mRoot = std::make_unique<Entity>();
			mRoot->setParentScene(*this);
		}

		virtual bool initialize() { 
			return mRoot->initialize(); 
		}

		inline Entity& getRoot() { 
			return *mRoot.get(); 
		}

		inline Game& getGame() { 
			return mGame; 
		}
	};
}