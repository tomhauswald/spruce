#pragma once

#include "Entity.h"

namespace spruce {

	class Game;

	class Scene {
	private:
		Entity mRoot;
		Game& mGame;

	public:
		Scene(Game& game) :
			mGame(game) {
			mRoot.setParentScene(*this);
		}

		virtual bool initialize() { 
			return mRoot.initialize(); 
		}

		inline Entity& getRoot() { 
			return mRoot;
		}

		inline Game& getGame() { 
			return mGame; 
		}
	};
}