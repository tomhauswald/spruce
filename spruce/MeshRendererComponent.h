#pragma once

#include "Component.h"
#include "Mesh.h"
#include "GLTexture.h"
#include "GLShaderProgram.h"
#include "Entity.h"
#include "CameraComponent.h"

namespace spruce {

	template<class MeshType>
	class MeshRendererComponent : public Component {
	private:
		MeshType* mMesh;
		GLShaderProgram* mShaderProgram;
		CameraComponent* mCamera;
		std::string mMVPUniformName;

	public:
		inline GLShaderProgram& getShaderProgram() {
			return *mShaderProgram;
		}

		inline void setMesh(MeshType* mesh) { 
			mMesh = mesh; 
		}

		inline void setShaderProgram(GLShaderProgram& program) {
			mShaderProgram = &program; 
		}

		inline void setCamera(CameraComponent& camera) {
			mCamera = &camera;
		}

		inline void setMVPUniformName(std::string name) {
			mMVPUniformName = name;
		}

		virtual void draw() override {
			if (mCamera != nullptr && mShaderProgram != nullptr && mMesh != nullptr) {
				mShaderProgram->use();
				mShaderProgram->getUniformVar(mMVPUniformName).store(mCamera->getViewProjMatrix() * mOwner->getTransform().getModelMatrix());

				mMesh->getVAO().bind();
				glDrawElements(GL_TRIANGLES, mMesh->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
				glUnbindVertexArray();
			}
		}
	};
}