#include "CameraComponent.h"
#include "Entity.h"

namespace spruce {

	void CameraComponent::update(float dt) {
		auto& ownerTransform = mOwner->getTransform();
		auto view = glm::lookAtLH(
			ownerTransform.getPosition(),
			ownerTransform.getPosition() + ownerTransform.getForward(),
			ownerTransform.getUp()
		);
		auto projection = glm::perspectiveLH(
			glm::radians(mFovDegrees),
			mAspectRatio,
			0.1f,
			100.0f
		);
		mViewProjMatrix = projection * view;
	}
}