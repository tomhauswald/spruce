#include "spruce_textured_mesh_renderer_component.h"

namespace spruce {

	void TxrMeshRendererComponent::draw() {
		if (mTexture == nullptr) return;
		if (mTextureUniformName.length() == 0) return;
		mTexture->bind(0);
		getShaderProgram().getUniformVar(mTextureUniformName).store(0);
		MeshRendererComponent<TexturedMesh>::draw();
	}
}