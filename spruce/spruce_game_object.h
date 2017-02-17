#pragma once

#include "spruce_common.h"
#include <unordered_map>
#include <algorithm>
#include "spruce_component.h"

namespace spruce {
	class Scene;	
	class Game_Object;

	class Game_Object {
	protected:
		bool alive_;

		Scene* scene_;
		Game_Object* parent_;
		std::unordered_map<std::string, std::unique_ptr<Game_Object>> children_;
		std::unordered_map<std::string, std::unique_ptr<Component>> components_;

	public:
		inline Scene* scene() { return scene_; }

		inline Game_Object* parent() { return parent_; }

		inline void set_scene(Scene* scene) { scene_ = scene; }

		inline void set_parent(Game_Object* parent) { parent_ = parent; }

		inline bool alive() const { return alive_; }


		// Kills this game object and all its children.
		void kill();

		// Initializes all components.
		bool initialize();

		// Updates all components followed by all children.
		void update(float dt);

		// Draws all components followed by all children.
		void draw();


		// Adds a component.
		Component* add_component(std::string const& name, std::unique_ptr<Component> component);

		// Returns a component by name.
		Component* component(std::string const& name);

		// Deletes a component by name.
		void delete_component(std::string const& name);

		// Decouples a component by name, 
		// allowing it to be added to another game object.
		std::unique_ptr<Component> decouple_component(std::string const& name);


		// Adds a child game object.
		Game_Object* add_child(std::string const& name, std::unique_ptr<Game_Object> child);

		// Returns a child game object by name.
		Game_Object* child(std::string const& name);

		// Deletes a child game object by name.
		void delete_child(std::string const& name);

		// Decouples a child game object by name, 
		// allowing it to be reparented to another game object.
		std::unique_ptr<Game_Object> decouple_child(std::string const& name);
	};
}