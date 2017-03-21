#include "spruce_transform.h"
#include "spruce_game_object.h"

namespace spruce {

	void Transform::decompose_matrix(mat4 const& matrix, vec3& pos, quat& rot, vec3& scl) {
		static vec3 skew;
		static vec4 persp;
		decompose(matrix, scl, rot, pos, skew, persp);
	}

	void Transform::update_transform() {
		local_matrix_ =
			glm::translate(local_position_) *
			glm::eulerAngleXYZ(local_rotation_.x, local_rotation_.y, local_rotation_.z) *
			glm::scale(local_scale_);

		if (parent_ != nullptr) {
			world_matrix_ = local_matrix_ * parent_matrix_;
		}
		else {
			world_matrix_ = local_matrix_;
		}

		object_->transform_changed();
	}

	void Transform::parent_transform_changed() {
		if (parent_ != nullptr) {
			parent_matrix_ = parent_->world();
			decompose_matrix(parent_matrix_, parent_position_, parent_rotation_, parent_scale_);
		}
		else {
			parent_position_ = vec3 {};
			parent_rotation_ = quat {};
			parent_scale_ = vec3 { 1, 1, 1 };
			parent_matrix_ = mat4 { 1.0f };
		}

		update_transform();
	}
}