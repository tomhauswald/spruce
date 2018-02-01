#include "FreeRoamCameraComponent.h"
#include "Input.h"
#include "Entity.h"

namespace spruce {

	void FreeRoamCameraComponent::update(float dt) {
		
		if (Input::isKeyDown(GLFW_KEY_W)) {
			mOwner->getTransform().getPosition() += mOwner->getTransform().getForward() * mSpeed * dt;
		}

		else if (Input::isKeyDown(GLFW_KEY_S)) {
			mOwner->getTransform().getPosition() -= mOwner->getTransform().getForward() * mSpeed * dt;
		}

		if (Input::isKeyDown(GLFW_KEY_D)) {
			mOwner->getTransform().getPosition() += mOwner->getTransform().getRight() * mSpeed * dt;
		}

		else if (Input::isKeyDown(GLFW_KEY_A)) {
			mOwner->getTransform().getPosition() -= mOwner->getTransform().getRight() * mSpeed * dt;
		}

		CameraComponent::update(dt);
	}
}