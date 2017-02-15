#include "spruce_game_object.h"
#include "spruce_game.h"

namespace spruce {

	Game_Object::Game_Object(Game* game, Game_Object* parent)
		: id_(parent == nullptr ? 0 : game->find_unused_id()),
		flags_(0ul),
		game_(game),
		parent_(parent_),
		alive_(true) {
	}
	
	void Game_Object::kill() {
		// Kill all children, as well.
		for (auto& child : children_) {
			child.second->kill();
		}

		// Mark as dead.
		// This will lead to our id becoming available again.
		alive_ = false;
	}

	void Game_Object::update(float dt) {
		std::vector<uint64_t> freed_ids;
		for (auto& child : children_) {
			if (child.second->alive_) {
				child.second->update(dt);
			} else {
				freed_ids.push_back(child.second->id_);
			}
		}

		for (auto id : freed_ids) {
			game_->yield_id(id);
			children_.erase(id);
		}
	}

	void Game_Object::draw() {
		for (auto& child : children_) {
			child.second->draw();
		}
	}

	void Game_Object::add_child(std::unique_ptr<Game_Object> child) {
		auto id = child->id_;
		children_.emplace(id, std::move(child));
	}

	void Game_Object::remove_child(uint64_t id) {
		children_.erase(id);
	}
}