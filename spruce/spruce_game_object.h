#pragma once

#include "spruce_common.h"
#include <unordered_map>
#include <algorithm>

namespace spruce {
	class Game;	
	class Game_Object;
	using Game_Object_Map = std::unordered_map<uint64_t, std::unique_ptr<Game_Object>>;

	class Game_Object {
	protected:
		uint64_t id_;
		uint64_t flags_;
		Game* game_;
		Game_Object* parent_;
		Game_Object_Map children_;
		bool alive_;

	public:
		Game_Object(Game* game, Game_Object* parent);

		inline uint64_t id() const { return id_; }
		inline uint64_t flags() const { return flags_; }
		inline Game const* game() const { return game_; }
		inline Game_Object const* parent() const { return parent_; }
		inline Game_Object_Map const& children() const { return children_; }
		inline Game_Object_Map& children() { return children_; }
		inline bool alive() const { return alive_; }

		void kill();

		virtual void update(float dt);
		virtual void draw();

		void add_child(std::unique_ptr<Game_Object> child);
		void remove_child(uint64_t id);
	};
}