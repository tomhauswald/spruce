#pragma once

#include "spruce_component.h"
#include "spruce_mesh.h"
#include "spruce_opengl_texture.h"
#include "spruce_opengl_program.h"
#include "spruce_entity.h"

namespace spruce {

	template<class MeshType>
	class MeshRendererComponent : public Component {
	private:
		MeshType* mMesh;
		GLShaderProgram* mShaderProgram;
		mat4 mViewProjMat;
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

		inline void setViewProjMat(mat4 matrix) { 
			mViewProjMat = matrix; 
		}

		inline void setMVPUniformName(std::string name) {
			mMVPUniformName = name;
		}

		virtual void draw() override {
			if (mShaderProgram != nullptr && mMesh != nullptr) {
				mShaderProgram->use();
				mShaderProgram->getUniformVar(mMVPUniformName).store(mViewProjMat * mOwner->getTransform().getModelMatrix());

				mMesh->getVAO().bind();
				glDrawElements(GL_TRIANGLES, mMesh->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
				glUnbindVertexArray();
			}
		}
	};
}