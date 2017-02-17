#pragma once

#include "spruce_component.h"
#include "spruce_mesh.h"
#include "spruce_opengl_texture.h"
#include "spruce_opengl_program.h"

namespace spruce {

	template<class MeshType>
	class Mesh_Renderer_Component : public Component {
	private:
		MeshType* mesh_;
		OpenGL_Program* program_;

	protected:
		virtual bool prepare(OpenGL_Program* program) { return true; }

	public:
		inline void set_mesh(MeshType* mesh) { mesh_ = mesh; }
		inline void set_program(OpenGL_Program* program) { program_ = program; }

		void draw() override {
			Log::msg << "Drawing mesh.\n";

			if (program_ && mesh_) {
				program_->use();
				if (!prepare(program_)) return;

				mesh_->vertex_array().bind();
				glDrawElements(GL_TRIANGLES, mesh_->indices().size(), GL_UNSIGNED_SHORT, nullptr);
				glUnbindVertexArray();
			}
		}
	};

	class Textured_Mesh_Renderer_Component : public Mesh_Renderer_Component<Textured_Mesh> {
	private:
		OpenGL_Texture* texture_;
		std::string texture_uniform_name_;

	protected:
		virtual bool prepare(OpenGL_Program* program) override {
			if (!texture_) return false;
			if (texture_uniform_name_.length() == 0) return false;
			texture_->bind(0);
			program->uniform(texture_uniform_name_)->store(0);
			return true;
		}

	public:
		inline void set_texture(OpenGL_Texture* texture) { texture_ = texture; }
		inline void set_texture_uniform_name(std::string const& name) { texture_uniform_name_ = name; }
	};
}