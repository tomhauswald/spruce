#include "CameraComponent.h"
#include "Entity.h"
#include "Log.h"

namespace spruce {

	void printMat4(mat4 const& m) {
		auto row0 = glm::row(m, 0);
		auto row1 = glm::row(m, 1);
		auto row2 = glm::row(m, 2);
		auto row3 = glm::row(m, 3);

		Log::msg.printf(
			"[%f %f %f %f\n[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n\n",
			row0[0], row0[1], row0[2], row0[3],
			row1[0], row1[1], row1[2], row1[3],
			row2[0], row2[1], row2[2], row2[3],
			row3[0], row3[1], row3[2], row3[3]
		);
	}

	void printVec3(vec3 const&v) {
		Log::msg.printf(
			"(%f %f %f)\n\n",
			v[0], v[1], v[2]
		);
	}

	void CameraComponent::update(float dt) {
		
		printMat4(mOwner->getTransform().getModelMatrix());
		printVec3(mOwner->getTransform().getRight());
		printVec3(mOwner->getTransform().getForward());

		auto& ownerTransform = mOwner->getTransform();
		auto view = glm::lookAtLH(
			ownerTransform.getPosition(),
			ownerTransform.getPosition() + ownerTransform.getForward(),
			{ 0,1,0 }
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