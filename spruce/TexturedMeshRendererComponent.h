#pragma once

#include "MeshRendererComponent.h"

namespace spruce {

	class TxrMeshRendererComponent : public MeshRendererComponent<TexturedMesh> {
	private:
		GLTexture* mTexture;
		std::string mTextureUniformName;

	public:
		inline void setTexture(GLTexture& texture) {
			mTexture = &texture; 
		}
		
		inline void setTextureUniformName(std::string name) { 
			mTextureUniformName = name; 
		}

		virtual void draw() override;
	};
}