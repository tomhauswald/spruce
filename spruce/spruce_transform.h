#pragma once

#include "spruce_component.h"
#include "spruce_graphics.h"

namespace spruce {
	class Game_Object;
	class Transform {
	private:
		Game_Object* object_;
		vec3 local_position_;
		vec3 local_orientation_;
		vec3 local_scale_;
		mat4 parent_to_model_;

		Transform* parent_;
		vec3 parent_position_;
		vec3 parent_orientation_;
		vec3 parent_scale_;
		mat4 world_to_parent_;

		vec3 world_position_;
		vec3 world_orientation_;
		vec3 world_scale_;
		mat4 world_to_model_;

		void decompose_matrix(mat4 const& matrix, vec3& pos, quat& rot, vec3& scl);
		void update_transform();

	public:
		Transform(Game_Object* object, Transform* parent = nullptr) :
			object_(object),
			local_position_ {},
			local_orientation_ {},
			local_scale_ { 1,1,1 },
			parent_to_model_ { 1.0f } {
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
		inline vec3 orientation() const {
			return world_orientation_;
		}

		inline float pitch() const {
			return world_orientation_.x;
		}

		inline float yaw() const {
			return world_orientation_.y;
		}

		inline float roll() const {
			return world_orientation_.z;
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


		// Get local orientation.
		inline vec3 local_orientation() const {
			return local_orientation_;
		}

		inline float local_pitch() const {
			return local_orientation_.x;
		}

		inline float local_yaw() const {
			return local_orientation_.y;
		}

		inline float local_roll() const {
			return local_orientation_.z;
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
		inline void set_local_orientation(vec3 const& orientation) {
			local_orientation_ = orientation;
			update_transform();
		}

		inline void set_local_orientation(float pitch, float yaw, float roll) {
			set_local_orientation({ pitch, yaw, roll });
		}

		inline void set_local_pitch(float pitch) {
			set_local_orientation(pitch, local_yaw(), local_roll());
		}

		inline void set_local_yaw(float yaw) {
			set_local_orientation(local_pitch(), yaw, local_roll());
		}

		inline void set_local_roll(float roll) { 
			set_local_orientation(local_pitch(), local_yaw(), roll);
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


		// Get parent transform matrix.
		inline mat4 const& parent_matrix() const {
			return world_to_parent_;
		}
		
		// Get local transform matrix.
		inline mat4 const& local_matrix() const {
			return parent_to_model_; 
		}

		// Get world transform matrix.
		inline mat4 const& model_matrix() const { 
			return world_to_model_; 
		}


		// Notify this transform of a change of its parent's transform.
		// Updates the parent matrix and combined transform, accordingly.
		void parent_transform_changed();
	};
}