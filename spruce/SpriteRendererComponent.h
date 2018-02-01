#pragma once

#include "TexturedMeshRendererComponent.h"
#include "GLTextureManager.h"

namespace spruce {
	class SpriteRendererComponent : public TxrMeshRendererComponent {
	private:
		static std::unique_ptr<TexturedMesh> sQuadMesh;

	public:
		SpriteRendererComponent() {
			if (sQuadMesh == nullptr) {
				sQuadMesh = std::make_unique<TexturedMesh>();
				sQuadMesh->initialize();

				auto& vertices = sQuadMesh->getVertices();

				vertices.push_back({
					{ 0.5f, 0.5f, -0.5f },
					{ 0, 0, -1 },
					{ 1, 1, 0 },
					{ 0, 0 }
				});

				vertices.push_back({
					{ 0.5f, -0.5f, -0.5f },
					{ 0, 0, -1 },
					{ 1, 1, 0 },
					{ 0, 1 }
				});

				vertices.push_back({
					{ -0.5f, -0.5f, -0.5f },
					{ 0, 0, -1 },
					{ 1, 1, 0 },
					{ 1, 1 }
				});

				vertices.push_back({
					{ -0.5f, 0.5f, -0.5f },
					{ 0, 0, -1 },
					{ 1, 1, 0 },
					{ 1, 0 }
				});

				auto& indices = sQuadMesh->getIndices();
				indices.insert(indices.end(), {
					0, 1, 2,
					2, 3, 0
				});

				sQuadMesh->updateMeshData();
			}

			setMesh(sQuadMesh.get());
		}
	};

	std::unique_ptr<TexturedMesh> SpriteRendererComponent::sQuadMesh;
}