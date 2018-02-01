#pragma once

#include "CameraComponent.h"

namespace spruce {
	
	class FreeRoamCameraComponent : public CameraComponent {
	private:
		float mSpeed;

	public:
		FreeRoamCameraComponent(float aspectRatio, float fovDegrees, float speed) :
			CameraComponent(aspectRatio, fovDegrees),
			mSpeed(speed) {
		}
		
		void update(float dt);
	};
}