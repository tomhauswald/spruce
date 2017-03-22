#include "spruce_transform.h"
#include "spruce_game_object.h"

namespace spruce {

	void Transform::decompose_matrix(mat4 const& matrix, vec3& pos, quat& rot, vec3& scl) {
		static vec3 skew;
		static vec4 persp;
		decompose(matrix, scl, rot, pos, skew, persp);
	}

	void Transform::update_transform() {
		parent_to_model_ =
			glm::translate(local_position_) *
			glm::rotate(local_orientation_.x, vec3 { 1, 0, 0 }) *
			glm::rotate(local_orientation_.y, vec3 { 0, 1, 0 }) *
			glm::rotate(local_orientation_.z, vec3 { 0, 0, 1 }) *
			glm::scale(local_scale_);

		if (parent_ != nullptr) {
			world_to_model_ = parent_to_model_ * world_to_parent_;
		}
		else {
			world_to_model_ = parent_to_model_;
			decompose_matrix(world_to_model_, world_position_, world_orientation_, world_scale_);
		}

		object_->transform_changed();
	}

	void Transform::parent_transform_changed() {
		if (parent_ != nullptr) {
			world_to_parent_ = parent_->model_matrix();
			decompose_matrix(world_to_parent_, parent_position_, parent_orientation_, parent_scale_);
		}
		else {
			parent_position_ = vec3 {};
			parent_orientation_ = quat {};
			parent_scale_ = vec3 { 1, 1, 1 };
			world_to_parent_ = mat4 { 1.0f };
		}

		update_transform();
	}
}