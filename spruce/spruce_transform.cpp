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
			glm::translate(local_position_) 
			* glm::eulerAngleXYZ(
			    local_orientation_.x,
			    local_orientation_.y,
			    local_orientation_.z)
			* glm::scale(local_scale_);

		if (parent_ != nullptr) {
			world_to_model_ = parent_to_model_ * world_to_parent_;
		}
		else {
			world_to_model_ = parent_to_model_;

			quat orient;
			decompose_matrix(world_to_model_, world_position_, orient, world_scale_);
			world_orientation_ = eulerAngles(orient);
		}

		object_->transform_changed();
	}

	void Transform::parent_transform_changed() {
		if (parent_ != nullptr) {
			world_to_parent_ = parent_->model_matrix();

			quat orient;
			decompose_matrix(world_to_parent_, parent_position_, orient, parent_scale_);
			parent_orientation_ = eulerAngles(orient);
		}
		else {
			parent_position_ = vec3 { 0.0f };
			parent_orientation_ = vec3 { 0.0f };
			parent_scale_ = vec3 { 1, 1, 1 };
			world_to_parent_ = mat4 { 1.0f };
		}

		update_transform();
	}
}