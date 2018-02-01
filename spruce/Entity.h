#pragma once

#include "Common.h"
#include <unordered_map>
#include <algorithm>
#include "Component.h"
#include "Transform.h"

namespace spruce {

	class Scene;

	class Entity {
	protected:
		bool mAlive;
		std::string mName;
		Transform mTransform;
		Entity* mParent;
		Scene* mParentScene;
		std::vector<std::unique_ptr<Entity>> mChildren;
		std::vector<std::unique_ptr<Component>> mComponents;

	public:
		Entity() :
			mAlive(false),
			mName("unnamed"),
			mParent(nullptr),
			mParentScene(nullptr) {
		}

		Entity(std::string name, Entity* parent = nullptr, Scene* scene = nullptr) :
			mAlive(false),
			mName(name),
			mParent(parent),
			mParentScene(scene) {}

		inline Scene& getParentScene() { 
			return *mParentScene; 
		}

		inline std::string const& getName() { 
			return mName; 
		}

		inline Entity* getParent() {
			return mParent; 
		}

		inline void setParentScene(Scene& scene) { 
			mParentScene = &scene; 
		}

		inline void setParent(Entity& parent) {
			auto parentTransform = parent.getTransform();
			mParent = &parent;
		}

		inline bool isAlive() const { 
			return mAlive; 
		}

		inline Transform& getTransform() {
			return mTransform;
		}

		// Kills this game object and all its children.
		void kill();

		// Initializes all components.
		bool initialize();

		// Updates all components followed by all children.
		void update(float dt);

		// Draws all components followed by all children.
		void draw();

		// Adds an existing component.
		Component& addComponent(std::unique_ptr<Component> component);

		template<typename T>
		T& addComponentOfType() {
			return (T&) addComponent(std::make_unique<T>());
		}

		// Returns an existing component of the specified type.
		template<typename T>
		T& getComponentOfType() {
			auto iter = std::find_if(
				mComponents.begin(),
				mComponents.end(),
				[](auto const& c) {
				    return typeid(*c) == typeid(T);
			    }
			);
			panicIf(
				iter == mComponents.end(), 
				"No component of the requested type is attached to entity " + mName + "."
			);
			return *(T*)(iter->get());
		}
		Component* getComponentByName(std::string const& name);

		/// Destroy a component.

		void destroyComponent(Component& component);
		
		template<typename T>
		void destroyComponentOfType() {
			auto c = getComponentOfType<T>();
			if (c != nullptr) {
				destroyComponent(*c);
			}
		}

		void destroyComponentByName(std::string const& name);

		/// Remove a component, allowing it to be added to another game object.

		std::unique_ptr<Component> removeComponent(Component& component);

		template<typename T>
		std::unique_ptr<Component> removeComponentOfType() {
			removeComponent(getComponentOfType<T>());
		}

		std::unique_ptr<Component> removeComponentByName(std::string const& name);


		/// Manage child entities.

		Entity& addChild(std::unique_ptr<Entity> child);

		Entity* getChildByName(std::string const& name);

		void destroyChild(Entity& child);

		void destroyChildByName(std::string const& name);

		std::unique_ptr<Entity> removeChild(Entity& child);

		std::unique_ptr<Entity> removeChildByName(std::string const& name);
	};
}