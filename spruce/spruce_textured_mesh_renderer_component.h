#pragma once

#include "spruce_mesh_renderer_component.h"

namespace spruce {
	class Textured_Mesh_Renderer_Component : public Mesh_Renderer_Component<Textured_Mesh> {
	private:
		OpenGL_Texture* texture_;
		std::string texture_uniform_name_;

	protected:
		virtual bool prepare(OpenGL_Program* program) override;

	public:
		inline void set_texture(OpenGL_Texture* texture) { texture_ = texture; }
		inline void set_texture_uniform_name(std::string const& name) { texture_uniform_name_ = name; }
	};
}