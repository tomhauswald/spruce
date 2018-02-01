#pragma once

#include "Common.h"
#include <optional>

namespace spruce {

	class Entity;

	class Component {

	protected:
		std::string mName;
		Entity* mOwner;

	public:
		Component() 
			: mName("unnamed"),
			  mOwner(nullptr) { 
		};

		Component(Entity* owner, std::string name)
			: mName(name),
			  mOwner(owner) { 
		}

		inline std::string const& getName() { 
			return mName; 
		}

		inline Entity& getOwner() {
			return *mOwner; 
		}
		
		inline void setOwner(Entity& owner) {
			mOwner = &owner; 
		}

		virtual bool initialize() {
			return true;
		}

		virtual void update(float dt) {
		}

		virtual void draw() {
		}
	};
}