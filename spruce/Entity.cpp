#include "Entity.h"
#include "Game.h"

namespace spruce {

	void Entity::kill() {

		// Kill all children, as well.
		for (auto& child : mChildren) {
			child->kill();
		}

		// Mark as dead.
		mAlive = false;
	}

	bool Entity::initialize() {

		// Init components.
		for (auto& component : mComponents) {
			if (!component->initialize()) {
				Log::err.printf("Initialization of component '%s' failed.\n", component->getName());
				return false;
			}
		}

		// Init children.
		for (auto& child : mChildren) {
			if (!child->initialize()) {
				Log::err.printf("Initialization of child '%s' failed.\n", child->getName());
				return false;
			}
		}

		mAlive = true;
		return true;
	}

	void Entity::update(float dt) {

		// Update components.
		for (auto& component : mComponents) {
			component->update(dt);
		}

		// Update alive children.
		std::vector<Entity*> childrenToErase;
		for (auto& child : mChildren) {
			if (child->isAlive()) {
				child->update(dt);
			} else {
				childrenToErase.push_back(child.get());
			}
		}

		// Erase dead children.
		for (auto& child : childrenToErase) {
			destroyChild(*child);
		}
	}

	void Entity::draw() {

		// Draw components.
		for (auto& component : mComponents) {
			component->draw();
		}

		// Draw children.
		for (auto& child : mChildren) {
			child->draw();
		}
	}

	Component& Entity::addComponent(std::unique_ptr<Component> component) {
		mComponents.push_back(std::move(component));
		mComponents.back()->setOwner(*this);
		return *mComponents.back();
	}

	Component* Entity::getComponentByName(std::string const& name) {
		return std::find_if(
			mComponents.begin(), 
			mComponents.end(), [&](auto& c) {
			    return c->getName() == name; 
		    }
		)->get();
	}

	std::unique_ptr<Component> Entity::removeComponent(Component& component) {
		auto iter = std::find_if(
			mComponents.begin(), 
			mComponents.end(), 
			[&](std::unique_ptr<Component>& c) {
				return c.get() == &component; 
			}
		);
		if (iter != mComponents.end()) {
			auto ret = std::move(*iter);
			mComponents.erase(iter);
			return std::move(ret);
		}
		else {
			return nullptr;
		}
	}

	std::unique_ptr<Component> Entity::removeComponentByName(std::string const& name) {
		auto iter = std::find_if(
			mComponents.begin(), 
			mComponents.end(), 
			[&](auto& c) {
				return c->getName() == name; 
			}
		);
		if (iter != mComponents.end()) {
			auto ret = std::move(*iter);
			mComponents.erase(iter);
			return std::move(ret);
		}
		else {
			return nullptr;
		}
	}

	void Entity::destroyComponent(Component& component) {
		auto iter = std::find_if(
			mComponents.begin(), 
			mComponents.end(), 
			[&](std::unique_ptr<Component>& c) {
				return c.get() == &component;
			}
		);
		if (iter != mComponents.end()) {
			mComponents.erase(iter);
		}
	}

	void Entity::destroyComponentByName(std::string const& name) {
		auto iter = std::find_if(
			mComponents.begin(), 
			mComponents.end(), 
			[&](auto& c) {
				return c->getName() == name; 
			}
		);
		if (iter != mComponents.end()) {
			mComponents.erase(iter);
		}
	}

	Entity& Entity::addChild(std::unique_ptr<Entity> child) {
		mChildren.push_back(std::move(child));
		mChildren.back()->setParent(*this);
		return *mChildren.back();
	}

	Entity* Entity::getChildByName(std::string const& name) {
		return std::find_if(
			mChildren.begin(),
			mChildren.end(), 
			[&](auto& c) {
				return c->getName() == name;
			}
		)->get();
	}

	std::unique_ptr<Entity> Entity::removeChild(Entity& child) {
		auto iter = std::find_if(
			mChildren.begin(), 
			mChildren.end(), 
			[&](std::unique_ptr<Entity>& c) {
				return c.get() == &child;
			}
		);
		if (iter != mChildren.end()) {
			auto ret = std::move(*iter);
			mChildren.erase(iter);
			return std::move(ret);
		}
		else {
			return nullptr;
		}
	}

	std::unique_ptr<Entity> Entity::removeChildByName(std::string const& name) {
		auto iter = std::find_if(
			mChildren.begin(), 
			mChildren.end(), 
			[&](auto& c) {
				return c->getName() == name; 
			}
		);
		if (iter != mChildren.end()) {
			auto ret = std::move(*iter);
			mChildren.erase(iter);
			return std::move(ret);
		}
		else {
			return nullptr;
		}
	}

	void Entity::destroyChild(Entity& child) {
		auto iter = std::find_if(
			mChildren.begin(), 
			mChildren.end(), 
			[&](std::unique_ptr<Entity>& c) {
				return c.get() == &child; 
			}
		);
		if (iter != mChildren.end()) {
			mChildren.erase(iter);
		}
	}

	void Entity::destroyChildByName(std::string const& name) {
		auto iter = std::find_if(
			mChildren.begin(), 
			mChildren.end(), 
			[&](auto& c) {
				return c->getName() == name;
			}
		);
		if (iter != mChildren.end()) {
			mChildren.erase(iter);
		}
	}
}