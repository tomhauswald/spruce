#pragma once

#include <glm/gtc/matrix_access.hpp>

#include "Component.h"
#include "Graphics.h"

namespace spruce {

	class Transform {
	private:
		vec3 mPosition;
		vec3 mRotation;
		vec3 mScale;
		mat4 mModelMatrix;

		bool mDirtyFlag;
		void decomposeMatrix(mat4 const& matrix, vec3& pos, quat& rot, vec3& scl);

	public:
		Transform() :
			mPosition(0,0,0),
			mRotation(0,0,0),
			mScale(1,1,1),
			mModelMatrix(1.0f),
			mDirtyFlag(true) {
		}

		void updateTransform();

		inline vec3& getPosition() {
			mDirtyFlag = true;
			return mPosition;
		}

		inline vec3& getRotation() {
			mDirtyFlag = true;
			return mRotation;
		}

		inline vec3& getScale() {
			mDirtyFlag = true;
			return mScale;
		}

		inline vec3 const& getPosition() const {
			return mPosition;
		}

		inline vec3 const& getRotation() const {
			return mRotation;
		}

		inline vec3 const& getScale() const {
			return mScale;
		}

		inline void setPosition(vec3 position) {
			mPosition = position;
			mDirtyFlag = true;
		}

		inline void setRotation(vec3 rotation) {
			mRotation = rotation;
			mDirtyFlag = true;
		}

		inline void setScale(vec3 scale) {
			mScale = scale; 
			mDirtyFlag = true;
		}
		
		inline mat4 const& getModelMatrix() { 
			if (mDirtyFlag) {
				updateTransform();
				mDirtyFlag = false;
			}
			return mModelMatrix; 
		}

		inline vec3 getForward() {
			auto col = glm::column(getModelMatrix(), 2);
			return vec3(col.x, col.y, col.z);
		}

		inline vec3 getRight() {
			return glm::cross({ 0,1,0 }, getForward());
		}
	};
}