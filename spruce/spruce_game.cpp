#include "spruce_game.h"

namespace spruce {
	Game::Game(OpenGL_Window_Settings const& window_settings, 
			   OpenGL_Context_Settings const& context_settings)
	: window_(window_settings, context_settings),
	  min_used_id_(0),
	  max_used_id_(0) {

		position_texture_ = std::make_unique<OpenGL_Texture>();
		position_texture_->upload_bitmap_data(
		Bitmap {
			window_.width(),
			window_.height()
		});

		normal_texture_ = std::make_unique<OpenGL_Texture>();
		normal_texture_->upload_bitmap_data(
		Bitmap {
			window_.width(),
			window_.height()
		});

		albedo_texture_ = std::make_unique<OpenGL_Texture>();
		albedo_texture_->upload_bitmap_data(
		Bitmap {
			window_.width(),
			window_.height()
		});

		geometry_fbo_ = std::make_unique<OpenGL_Frame_Buffer>();
		geometry_fbo_->attach_texture(GL_COLOR_ATTACHMENT0, *position_texture_);
		geometry_fbo_->attach_texture(GL_COLOR_ATTACHMENT1, *normal_texture_);
		geometry_fbo_->attach_texture(GL_COLOR_ATTACHMENT2, *albedo_texture_);

		// Load the shader program used to render fullscreen quads.
		fsq_program_ = std::make_unique<OpenGL_Program>("fsq");
		{
			OpenGL_Shader vs { OpenGL_Shader_Type::Vertex };
			vs.compile_from_source("fsq.vert");

			OpenGL_Shader fs { OpenGL_Shader_Type::Fragment };
			fs.compile_from_source("fsq.frag");

			fsq_program_->attach_shader(vs);
			fsq_program_->attach_shader(fs);
			fsq_program_->link();
			fsq_program_->add_uniform("uPositionTexture");
			fsq_program_->add_uniform("uNormalTexture");
			fsq_program_->add_uniform("uAlbedoTexture");
		}

		// Construct fsq mesh.
		fsq_mesh = std::make_unique<FSQ_Mesh>();
		fsq_mesh->initialize();
		fsq_mesh->vertices().insert(
		fsq_mesh->vertices().end(), {
			{ { -1.0f,  1.0f } },
			{ { -1.0f, -1.0f } },
			{ { 1.0f, -1.0f } },
			{ { 1.0f,  1.0f } }
		});
		fsq_mesh->indices().insert(
		fsq_mesh->indices().end(), {
			0, 1, 2,
			2, 3, 0
		});
		fsq_mesh->update();

		scene_root_ = std::make_unique<Game_Object>(this, nullptr);
	}

	void Game::update(float dt) {
		window_.poll_events();
		scene_root_->update(dt);
	}

	void Game::draw() {
		window_.clear_buffer({ 0.0f, 0.0f, 0.0f });

		// Prepare rendering to G-buffer.
		geometry_fbo_->bind();
		geometry_fbo_->clear({ 0, 0, 0 });

		scene_root_->draw();

		// Restore default FBO.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw fullscreen quad.
		fsq_program_->use();
		fsq_program_->uniform("uPositionTexture")->store(0);
		fsq_program_->uniform("uNormalTexture")->store(1);
		fsq_program_->uniform("uAlbedoTexture")->store(2);
		position_texture_->bind(0);
		normal_texture_->bind(1);
		albedo_texture_->bind(2);
		fsq_mesh->draw();

		window_.swap_buffers();
	}

	void Game::run() {
		static double current_time = glfwGetTime();
		static double last_frame_time = glfwGetTime();

		panic_if(!initialize(), "Game initialization failed.");
		running_ = true;

		while (running_) {
			current_time = glfwGetTime();
			auto dt = static_cast<float>(glfwGetTime() - last_frame_time);
			last_frame_time = current_time;

			update(glfwGetTime() - last_frame_time);
			draw();

			running_ &= !window_.should_close();
		}
	}

	uint64_t Game::find_unused_id() {
		if (min_used_id_ > 0) {
			return --min_used_id_;
		}

		else if (max_used_id_ < UINT64_MAX) {
			return ++max_used_id_;
		}

		else {
			for (auto id = min_used_id_ + 1; id < max_used_id_; ++id) {
				// check if id is in use.
				return id;
			}
		}
	}

	void Game::yield_id(uint64_t id) {
		// Root node was killed.
		if (id == 0) {
			min_used_id_ = max_used_id_ = 0;
		}
		
		// Free up minimum id.
		else if (id == min_used_id_) ++min_used_id_;

		// Free up maximum id.
		else if (id == max_used_id_) --max_used_id_;
	}
}