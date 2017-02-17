#include "spruce_game_object.h"
#include "spruce_game.h"

namespace spruce {
	void Game_Object::kill() {

		// Kill all children, as well.
		for (auto& child : children_) {
			child.second->kill();
		}

		// Mark as dead.
		alive_ = false;
	}

	bool Game_Object::initialize() {

		// Init components.
		for (auto& component : components_) {
			if (!component.second->initialize()) {
				Log::err.printf("Initialization of component '%s' failed.\n", component.first);
				return false;
			}
		}

		// Init children.
		for (auto& child : children_) {
			if (!child.second->initialize()) {
				Log::err.printf("Initialization of child '%s' failed.\n", child.first);
				return false;
			}
		}

		alive_ = true;
		return true;
	}

	void Game_Object::update(float dt) {

		// Update components.
		for (auto& component : components_) {
			component.second->update(dt);
		}

		std::vector<std::string> dead_child_names;
		dead_child_names.reserve(children_.size());

		// Update alive children.
		for (auto& child_info : children_) {
			if (child_info.second->alive_) {
				child_info.second->update(dt);
			} else {
				dead_child_names.push_back(child_info.first);
			}
		}

		// Erase dead children.
		for (auto const& name : dead_child_names) {
			children_.erase(name);
		}
	}

	void Game_Object::draw() {

		// Draw components.
		for (auto& component : components_) {
			component.second->draw();
		}

		// Draw children.
		for (auto& child : children_) {
			child.second->draw();
		}
	}

	Component* Game_Object::add_component(std::string const& name, std::unique_ptr<Component> component) {
		components_[name] = std::move(component);
		components_[name]->set_owner(this);
		return this->component(name);
	}

	Component* Game_Object::component(std::string const& name) {
		return components_[name].get();
	}

	void Game_Object::delete_component(std::string const& name) {
		components_.erase(name);
	}

	std::unique_ptr<Component> Game_Object::decouple_component(std::string const& name) {
		auto component = std::move(components_[name]);
		components_.erase(name);
		return std::move(component);
	}

	Game_Object* Game_Object::add_child(std::string const& name, std::unique_ptr<Game_Object> child) {
		children_[name] = std::move(child);
		children_[name]->set_parent(this);
		children_[name]->set_scene(scene_);
		return this->child(name);
	}

	Game_Object* Game_Object::child(std::string const& name) {
		return children_[name].get();
	}

	void Game_Object::delete_child(std::string const& name) {
		children_.erase(name);
	}

	std::unique_ptr<Game_Object> Game_Object::decouple_child(std::string const& name) {
		auto child = std::move(children_[name]);
		children_.erase(name);
		return std::move(child);
	}
}