#include "Transform.h"
#include "Entity.h"

namespace spruce {

	void Transform::decomposeMatrix(mat4 const& matrix, vec3& pos, quat& rot, vec3& scl) {
		static vec3 skew;
		static vec4 persp;
		decompose(matrix, scl, rot, pos, skew, persp);
	}

	void Transform::updateTransform() {
		mModelMatrix =
		glm::translate(
			mPosition
		) * glm::eulerAngleXYZ(
			glm::radians(mRotation.x),
			glm::radians(mRotation.y),
			glm::radians(mRotation.z)
		) * glm::scale(
			mScale
		);
	}
}