#pragma once

#include "spruce_component.h"
#include "spruce_graphics.h"

namespace spruce {
	class Game_Object;
	class Transform {
	private:
		Game_Object* object_;
		vec3 local_position_;
		quat local_rotation_;
		vec3 local_scale_;
		mat4 local_matrix_;

		Transform* parent_;
		vec3 parent_position_;
		quat parent_rotation_;
		vec3 parent_scale_;
		mat4 parent_matrix_;

		vec3 world_position_;
		quat world_rotation_;
		vec3 world_scale_;
		mat4 world_matrix_;

		void decompose_matrix(mat4 const& matrix, vec3& pos, quat& rot, vec3& scl);
		void update_transform();

	public:
		Transform(Game_Object* object, Transform* parent = nullptr) :
			object_(object) {
			set_parent(parent);
		}

		// Get world-space position.
		inline vec3 const& position() const { 
			return world_position_; 
		}

		inline float x() const {
			return world_position_.x;
		}

		inline float y() const { 
			return world_position_.y;
		}

		inline float z() const { 
			return world_position_.z;
		}


		// Get world-space rotation.
		inline quat const& rotation() const {
			return world_rotation_;
		}

		inline vec3 euler() const { 
			return glm::eulerAngles(world_rotation_);
		}

		inline float pitch() const { 
			return glm::pitch(world_rotation_);
		}

		inline float yaw() const {
			return glm::yaw(world_rotation_);
		}

		inline float roll() const { 
			return glm::roll(world_rotation_);
		}


		// Get world-space scale.
		inline vec3 const& scale() const { 
			return world_scale_;
		}

		inline float scale_x() const {
			return world_scale_.x;
		}

		inline float scale_y() const { 
			return world_scale_.y;
		}

		inline float scale_z() const { 
			return world_scale_.z;
		}


		// Get local position.
		inline vec3 const& local_position() const {
			return local_position_;
		}

		inline float local_x() const {
			return local_position_.x;
		}

		inline float local_y() const { 
			return local_position_.y;
		}

		inline float local_z() const {
			return local_position_.z;
		}


		// Get local rotation.
		inline quat const& local_rotation() const { 
			return local_rotation_;
		}

		inline vec3 local_euler() const { 
			return glm::eulerAngles(local_rotation_); 
		}

		inline float local_pitch() const { 
			return glm::pitch(local_rotation_); 
		}

		inline float local_yaw() const { 
			return glm::yaw(local_rotation_); 
		}

		inline float local_roll() const { 
			return glm::roll(local_rotation_); 
		}


		// Get local scale.
		inline vec3 const& local_scale() const { 
			return local_scale_;
		}

		inline float local_scale_x() const { 
			return local_scale_.x; 
		}

		inline float local_scale_y() const { 
			return local_scale_.y;
		}

		inline float local_scale_z() const { 
			return local_scale_.z; 
		}


		// Set local position.
		inline void set_local_position(vec3 const& position) { 
			local_position_ = position; 
			update_transform();
		}

		inline void set_local_position(float x, float y, float z) { 
			local_position_.x = x; 
			local_position_.y = y; 
			local_position_.z = z; 
			update_transform();
		}
		
		inline void set_local_x(float x) { 
			local_position_.x = x; 
			update_transform(); 
		}

		inline void set_local_y(float y) {
			local_position_.y = y; 
			update_transform();
		}
		
		inline void set_local_z(float z) {
			local_position_.z = z; 
			update_transform();
		}


		// Set local rotation.
		inline void set_local_rotation(quat const& rotation) { 
			local_rotation_ = rotation; 
			update_transform();
		}

		inline void set_local_rotation(float pitch, float yaw, float roll) { 
			local_rotation_ = glm::eulerAngleXYZ(pitch, yaw, roll); 
			update_transform();
		}

		inline void set_local_pitch(float pitch) { 
			local_rotation_ = glm::eulerAngleXYZ(pitch, local_yaw(), local_roll());
			update_transform();
		}

		inline void set_local_yaw(float yaw) {
			local_rotation_ = glm::eulerAngleXYZ(local_pitch(), yaw, local_roll());
			update_transform();
		}

		inline void set_local_roll(float roll) { 
			local_rotation_ = glm::eulerAngleXYZ(local_pitch(), local_yaw(), roll); 
			update_transform();
		}


		// Set local scale.
		inline void set_local_scale(vec3 const& scale) {
			local_scale_ = scale; 
			update_transform();
		}

		inline void set_local_scale(float x, float y, float z) { 
			local_scale_.x = x; 
			local_scale_.y = y; 
			local_scale_.z = z;
			update_transform();
		}

		inline void set_local_scale_x(float x) { 
			local_scale_.x = x; 
			update_transform();
		}
		
		inline void set_local_scale_y(float y) { 
			local_scale_.y = y; 
			update_transform();
		}
		
		inline void set_local_scale_z(float z) { 
			local_scale_.z = z; 
			update_transform();
		}


		// Get, set parent transform.
		inline Transform const* parent() const { 
			return parent_; 
		}

		inline Transform* parent() { 
			return parent_; 
		}

		inline void set_parent(Transform* parent) { 
			parent_ = parent;
			parent_transform_changed();
		}

		
		// Get local transform matrix.
		inline mat4 const& local() const {
			return local_matrix_; 
		}


		// Get world transform matrix.
		inline mat4 const& world() const { 
			return world_matrix_; 
		}


		// Notify this transform of a change of its parent's transform.
		// Updates the parent matrix and combined transform, accordingly.
		void parent_transform_changed();
	};
}