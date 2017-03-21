#include "spruce_textured_mesh_renderer_component.h"

namespace spruce {
	bool Textured_Mesh_Renderer_Component::prepare(OpenGL_Program* program) {
		if (!texture_) return false;
		if (texture_uniform_name_.length() == 0) return false;
		texture_->bind(0);
		program->uniform(texture_uniform_name_)->store(0);
		return true;
	}
}