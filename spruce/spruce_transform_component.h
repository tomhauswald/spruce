#pragma once

#include "spruce_component.h"
#include "spruce_graphics.h"

namespace spruce {
	class Transform_Component : public Component {
	private:
		vec3 local_position_;
		quat local_rotation_;
		vec3 local_scale_;
		mat4 local_matrix_;

		vec3 position_;
		quat rotation_;
		vec3 scale_;
		mat4 world_matrix_;

		Transform_Component* parent_;

	public:
		Transform_Component(Transform_Component* parent) :
			local_scale_ { 1,1,1 },
			local_matrix_ { 1.0f },
			parent_ { parent } {

			if (parent_ != nullptr) {
				world_matrix_ = parent_->world_matrix_;
				position_ = parent_->position_;
				rotation_ = parent_->rotation_;
				scale_ = parent_->scale_;
			}
			else {
				world_matrix_ = local_matrix_;
				position_ = local_position_;
				rotation_ = local_rotation_;
				scale_ = local_scale_;
			}
		}

		Transform_Component() :
			Transform_Component(nullptr) {}

		virtual void update(float dt) override {
			local_matrix_ =
				glm::translate(local_position_) *
				glm::eulerAngleXYZ(local_rotation_.x, local_rotation_.y, local_rotation_.z) *
				glm::scale(local_scale_);

			if (parent_ != nullptr) {
				world_matrix_ = parent_->world_matrix_ * local_matrix_;

				vec3 skew;
				vec4 perspective;
				glm::decompose(world_matrix_, scale_, rotation_, position_, skew, perspective);
			}
			else {
				world_matrix_ = local_matrix_;
			}
		}

		// Get world-space position.
		inline vec3 const& position() const { return position_; }
		inline float x() const { return position_.x; }
		inline float y() const { return position_.y; }
		inline float z() const { return position_.z; }

		// Get world-space rotation.
		inline quat const& rotation() const { return rotation_; }
		inline vec3 euler() const { return glm::eulerAngles(rotation_); }
		inline float pitch() const { return glm::pitch(rotation_); }
		inline float yaw() const { return glm::yaw(rotation_); }
		inline float roll() const { return glm::roll(rotation_); }

		// Get world-space scale.
		inline vec3 const& scale() const { return scale_; }
		inline float scale_x() const { return scale_.x; }
		inline float scale_y() const { return scale_.y; }
		inline float scale_z() const { return scale_.z; }

		// Get local position.
		inline vec3 const& local_position() const { return local_position_; }
		inline float local_x() const { return local_position_.x; }
		inline float local_y() const { return local_position_.y; }
		inline float local_z() const { return local_position_.z; }

		// Get local rotation.
		inline quat const& local_rotation() const { return local_rotation_; }
		inline vec3 local_euler() const { return glm::eulerAngles(local_rotation_); }
		inline float local_pitch() const { return glm::pitch(local_rotation_); }
		inline float local_yaw() const { return glm::yaw(local_rotation_); }
		inline float local_roll() const { return glm::roll(local_rotation_); }

		// Get local scale.
		inline vec3 const& local_scale() const { return local_scale_; }
		inline float local_scale_x() const { return local_scale_.x; }
		inline float local_scale_y() const { return local_scale_.y; }
		inline float local_scale_z() const { return local_scale_.z; }

		// Set local position.
		inline void set_local_position(vec3 const& position) { local_position_ = position; }
		inline void set_local_position(float x, float y, float z) { local_position_.x = x; local_position_.y = y; local_position_.z = z; }
		inline void set_local_x(float x) { local_position_.x = x; }
		inline void set_local_y(float y) { local_position_.y = y; }
		inline void set_local_z(float z) { local_position_.z = z; }

		// Set local rotation.
		inline void set_local_rotation(quat const& rotation) { local_rotation_ = rotation; }
		inline void set_local_rotation(float pitch, float yaw, float roll) { local_rotation_ = glm::eulerAngleXYZ(pitch, yaw, roll); }
		inline void set_local_pitch(float pitch) { local_rotation_ = glm::eulerAngleXYZ(pitch, local_yaw(), local_roll()); }
		inline void set_local_yaw(float yaw) { local_rotation_ = glm::eulerAngleXYZ(local_pitch(), yaw, local_roll()); }
		inline void set_local_roll(float roll) { local_rotation_ = glm::eulerAngleXYZ(local_pitch(), local_yaw(), roll); }

		// Set local scale.
		inline void set_local_scale(vec3 const& scale) { local_scale_ = scale; }
		inline void set_local_scale(float x, float y, float z) { local_scale_.x = x; local_scale_.y = y; local_scale_.z = z; }
		inline void set_local_scale_x(float x) { local_scale_.x = x; }
		inline void set_local_scale_y(float y) { local_scale_.y = y; }
		inline void set_local_scale_z(float z) { local_scale_.z = z; }

		// Get, set parent transform.
		inline Transform_Component const* parent() const { return parent_; }
		inline Transform_Component* parent() { return parent_; }
		inline void set_parent(Transform_Component* parent) { parent_ = parent; }

		// Get local transform matrix.
		inline mat4 const& local() const { return local_matrix_; }

		// Get world transform matrix.
		inline mat4 const& world() const { return world_matrix_; }
	};
}