#pragma once

#include "spruce_opengl_window.h"
#include "spruce_game_object.h"
#include "spruce_opengl_program.h"
#include "spruce_opengl_texture.h"
#include "spruce_opengl_frame_buffer.h"
#include "spruce_mesh.h"
#include "spruce_scene.h"
#include "spruce_renderer.h"

namespace spruce {
	class Game {
	private:
		bool running_;

		std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
		std::string active_scene_name_;

		std::unique_ptr<OpenGL_Window> window_;
		std::unique_ptr<Renderer> renderer_;

		bool initialize();
		void update(float dt);
		void draw();

	public:
		inline void set_window(std::unique_ptr<OpenGL_Window> window) {
			window_ = std::move(window);
		}

		inline void set_renderer(std::unique_ptr<Renderer> renderer) {
			renderer_ = std::move(renderer);
		}

		Scene* add_scene(std::string const& name, std::unique_ptr<Scene> scene) {
			scenes_[name] = std::move(scene);
			scenes_[name]->set_game(this);
			return scenes_[name].get();
		}

		inline void set_start_scene(std::string const& name) {
			active_scene_name_ = name;
		}

		inline void set_active_scene(std::string const& name) {
			active_scene_name_ = name;
			panic_if(!active_scene()->initialize(), "Failed to initialize scene '" + name + "'.");
		}

		inline Scene* active_scene() { 
			return scenes_[active_scene_name_].get(); 
		}

		inline OpenGL_Window const* window() const { return window_.get(); }

		void run();
	};
}