#include "spruce_deferred_renderer.h"
#include "spruce_game.h"

namespace spruce {

	Deferred_Geometry_Pass::Deferred_Geometry_Pass(Deferred_Renderer* renderer)
		: Render_Pass(renderer) {
	}

	void Deferred_Geometry_Pass::render(Scene* scene) {
		Log::msg << "Rendering geometry pass.\n";
		auto deferred_renderer = ((Deferred_Renderer*)renderer_);

		// Prepare rendering to G-buffer.
		deferred_renderer->geometry_fbo_->bind();
		deferred_renderer->geometry_fbo_->clear({ 0, 0, 0 });

		// Render scene to G-buffer.
		scene->root()->draw();
	}

	Deferred_Lighting_Pass::Deferred_Lighting_Pass(Deferred_Renderer* renderer)
		: Render_Pass(renderer) {

		// Load the shader program used to render FSQs.
		fsq_program_ = std::make_unique<OpenGL_Program>("fsq");
		{
			OpenGL_Shader vs { OpenGL_Shader_Type::Vertex };
			vs.compile_from_source("fsq.vert");

			OpenGL_Shader fs { OpenGL_Shader_Type::Fragment };
			fs.compile_from_source("fsq.frag");

			fsq_program_->attach_shader(vs);
			fsq_program_->attach_shader(fs);
			fsq_program_->link();
			fsq_program_->add_uniform("uPositionTexture").store(0);
			fsq_program_->add_uniform("uNormalTexture").store(1);
			fsq_program_->add_uniform("uAlbedoTexture").store(2);
		}

		// Construct FSQ mesh.
		fsq_mesh_ = std::make_unique<FSQ_Mesh>();
		{
			fsq_mesh_->initialize();
			fsq_mesh_->vertices().insert(
			fsq_mesh_->vertices().end(), {
				{ { -1.0f,  1.0f } },
				{ { -1.0f, -1.0f } },
				{ { 1.0f, -1.0f } },
				{ { 1.0f,  1.0f } }
			});
			fsq_mesh_->indices().insert(
			fsq_mesh_->indices().end(), {
				0, 1, 2,
				2, 3, 0
			});
			fsq_mesh_->update();
		}
	}

	void Deferred_Lighting_Pass::render(Scene* scene) {
		Log::msg << "Rendering light pass.\n";
		auto deferred_renderer = ((Deferred_Renderer*)renderer_);

		// Restore default FBO.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw fullscreen quad.
		fsq_program_->use();
		fsq_program_->uniform("uPositionTexture")->store(0);
		fsq_program_->uniform("uNormalTexture")->store(1);
		fsq_program_->uniform("uAlbedoTexture")->store(2);
		deferred_renderer->position_texture_->bind(0);
		deferred_renderer->normal_texture_->bind(1);
		deferred_renderer->albedo_texture_->bind(2);

		fsq_mesh_->vertex_array().bind();
		glDrawElements(GL_TRIANGLES, fsq_mesh_->indices().size(), GL_UNSIGNED_SHORT, nullptr);
		glUnbindVertexArray();
	}

	bool Deferred_Renderer::initialize() {

		position_texture_ = std::make_unique<OpenGL_Texture>();
		position_texture_->upload_bitmap_data(
			Bitmap {
			game_->window()->width(),
			game_->window()->height()
		});

		normal_texture_ = std::make_unique<OpenGL_Texture>();
		normal_texture_->upload_bitmap_data(
			Bitmap {
			game_->window()->width(),
			game_->window()->height()
		});

		albedo_texture_ = std::make_unique<OpenGL_Texture>();
		albedo_texture_->upload_bitmap_data(
			Bitmap {
			game_->window()->width(),
			game_->window()->height()
		});

		geometry_fbo_ = std::make_unique<OpenGL_Frame_Buffer>();
		geometry_fbo_->attach_texture(GL_COLOR_ATTACHMENT0, *position_texture_);
		geometry_fbo_->attach_texture(GL_COLOR_ATTACHMENT1, *normal_texture_);
		geometry_fbo_->attach_texture(GL_COLOR_ATTACHMENT2, *albedo_texture_);

		passes_.push_back(std::make_unique<Deferred_Geometry_Pass>(this));
		passes_.push_back(std::make_unique<Deferred_Lighting_Pass>(this));
		return true;
	}
}