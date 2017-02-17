#pragma once

#include "spruce_common.h"

namespace spruce {
	class Game_Object;

	class Component {
	protected:
		Game_Object* owner_;

	public:
		Component() = default;
		Component(Game_Object* owner) : owner_(owner) {}
		inline Game_Object* owner() { return owner_; }
		inline Game_Object const* owner() const { return owner_; }
		inline void set_owner(Game_Object* owner) { owner_ = owner; }

		virtual bool initialize() { return true; }
		virtual void update(float dt) { };
		virtual void draw() { };
	};
}