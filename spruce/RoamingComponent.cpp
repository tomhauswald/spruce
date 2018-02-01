#include "RoamingComponent.h"
#include "Input.h"
#include "Entity.h"

namespace spruce {

	void RoamingComponent::update(float dt) {

		auto& transform = mOwner->getTransform();
		auto& pos = transform.getPosition();
		auto forward = mOwner->getTransform().getForward();
		auto right = mOwner->getTransform().getRight();

		if (Input::isKeyDown(GLFW_KEY_W)) {
			pos += forward * mMoveSpeed * dt;
		}

		else if (Input::isKeyDown(GLFW_KEY_S)) {
			pos -= forward * mMoveSpeed * dt;
		}

		if (Input::isKeyDown(GLFW_KEY_D)) {
			pos += right * mMoveSpeed * dt;
		}

		else if (Input::isKeyDown(GLFW_KEY_A)) {
			pos -= right * mMoveSpeed * dt;
		}

		auto& rotation = transform.getRotation();
		auto delta = Input::getCursorDelta();

		auto euler = glm::eulerAngleXYZ(
			glm::radians(rotation.x),
			glm::radians(rotation.y),
			glm::radians(rotation.z)
		);
		euler *= glm::rotate<float>(glm::radians(mLookSpeed * delta.x * dt), vec3(0, 1, 0));
		euler *= glm::rotate<float>(glm::radians(mLookSpeed * delta.y * dt), right);
		
		float rx, ry, rz;
		glm::extractEulerAngleXYZ(euler, rx, ry, rz);

		rotation.x = glm::degrees(rx);
		rotation.y = glm::degrees(ry);
		rotation.z = glm::degrees(rz);
	}
}