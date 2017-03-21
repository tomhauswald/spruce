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
		mat4 view_matrix_;
		mat4 projection_matrix_;

	protected:
		virtual bool prepare(OpenGL_Program* program) { return true; }

	public:
		inline void set_mesh(MeshType* mesh) { mesh_ = mesh; }
		inline void set_program(OpenGL_Program* program) { program_ = program; }
		inline void set_view_matrix(mat4 const& matrix) { view_matrix_ = matrix; }
		inline void set_projection_matrix(mat4 const& matrix) { projection_matrix_ = matrix; }

		void draw() override {
			if (program_ && mesh_) {
				program_->use();
				program_->uniform("uModel")->store(owner_->transform()->world());
				program_->uniform("uView")->store(view_matrix_);
				program_->uniform("uProjection")->store(projection_matrix_);

				if (prepare(program_)) {
					mesh_->vertex_array().bind();
					glDrawElements(GL_TRIANGLES, mesh_->indices().size(), GL_UNSIGNED_SHORT, nullptr);
					glUnbindVertexArray();
				}
			}
		}
	};
}