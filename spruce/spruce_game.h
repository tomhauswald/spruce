#pragma once

#include "spruce_opengl_window.h"
#include "spruce_game_object.h"
#include "spruce_opengl_program.h"
#include "spruce_opengl_texture.h"
#include "spruce_opengl_frame_buffer.h"
#include "spruce_mesh.h"

namespace spruce {
	class Game {
	private:
		bool running_;
		std::unique_ptr<OpenGL_Texture> position_texture_;
		std::unique_ptr<OpenGL_Texture> normal_texture_;
		std::unique_ptr<OpenGL_Texture> albedo_texture_;
		std::unique_ptr<OpenGL_Frame_Buffer> geometry_fbo_;
		std::unique_ptr<OpenGL_Program> fsq_program_;
		std::unique_ptr<FSQ_Mesh> fsq_mesh;
		uint64_t min_used_id_, max_used_id_;
		
	protected:
		OpenGL_Window window_;
		std::unique_ptr<Game_Object> scene_root_;

	public:
		Game(OpenGL_Window_Settings const& window_settings,
			 OpenGL_Context_Settings const& context_settings);

		virtual bool initialize() = 0;
		void update(float dt);
		void draw();
		void run();

		inline OpenGL_Window const& window() const { return window_; }
		inline Game_Object const* scene_root() const { return scene_root_.get(); }
		inline Game_Object* scene_root() { return scene_root_.get(); }

		uint64_t find_unused_id();
		void yield_id(uint64_t id);
	};
}