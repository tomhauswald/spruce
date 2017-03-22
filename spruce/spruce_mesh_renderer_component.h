#pragma once

#include "spruce_component.h"
#include "spruce_mesh.h"
#include "spruce_opengl_texture.h"
#include "spruce_opengl_program.h"
#include "spruce_game_object.h"

namespace spruce {

	template<class MeshType>
	class Mesh_Renderer_Component : public Component {
	private:
		MeshType* mesh_;
		OpenGL_Program* program_;
		mat4 view_projection_matrix_;
		std::string mvp_uniform_name_;

	protected:
		virtual bool prepare(OpenGL_Program* program) { return true; }

	public:
		inline void set_mesh(MeshType* mesh) { mesh_ = mesh; }
		inline void set_program(OpenGL_Program* program) { program_ = program; }
		inline void set_view_projection_matrix(mat4 const& matrix) { view_projection_matrix_ = matrix; }
		inline void set_mvp_uniform_name(std::string const& name) { mvp_uniform_name_ = name; }

		void draw() override {
			if (program_ && mesh_) {
				program_->use();
				program_->uniform(mvp_uniform_name_)->store(view_projection_matrix_ * owner_->transform()->model_matrix());

				if (prepare(program_)) {
					mesh_->vertex_array().bind();
					glDrawElements(GL_TRIANGLES, mesh_->indices().size(), GL_UNSIGNED_SHORT, nullptr);
					glUnbindVertexArray();
				}
			}
		}
	};
}