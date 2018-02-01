#pragma once

#include "Graphics.h"
#include "Component.h"

namespace spruce {

	class CameraComponent : public Component {
	private:
		float mAspectRatio;
		float mFovDegrees;
		mat4 mViewProjMatrix;

	public:
		CameraComponent(float aspectRatio, float fovDegrees) :
			mAspectRatio(aspectRatio),
			mFovDegrees(fovDegrees),
			mViewProjMatrix(1.0f) {
		}

		inline mat4 const& getViewProjMatrix() const {
			return mViewProjMatrix;
		}

		void update(float dt) override;
	};
}