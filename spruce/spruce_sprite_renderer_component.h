#pragma once

#include "spruce_textured_mesh_renderer_component.h"
#include "spruce_opengl_textures.h"

namespace spruce {
	class Sprite_Renderer_Component : public Textured_Mesh_Renderer_Component {
	private:
		std::unique_ptr<Textured_Mesh> mesh_;

	public:
		Sprite_Renderer_Component() {
			mesh_ = std::make_unique<Textured_Mesh>();
			mesh_->initialize();

			auto& vertices = mesh_->vertices();

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

			auto& indices = mesh_->indices();
			indices.insert(indices.end(), {
				0, 1, 2,
				2, 3, 0
			});

			mesh_->update();
			set_mesh(mesh_.get());
		}
	};
}