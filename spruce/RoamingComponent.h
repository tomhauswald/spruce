#pragma once

#include "Component.h"

namespace spruce {
	
	class RoamingComponent : public Component {
	private:
		float mLookSpeed;
		float mMoveSpeed;

	public:
		RoamingComponent(float lookSpeed, float moveSpeed) :
			mLookSpeed(lookSpeed),
			mMoveSpeed(moveSpeed) {
		}
		
		void update(float dt);
	};
}