#pragma once

#include "spruce_renderer.h"
#include "spruce_opengl_texture.h"
#include "spruce_mesh.h"
#include "spruce_opengl_frame_buffer.h"
#include "spruce_opengl_program.h"
#include "spruce_mesh_renderer_component.h"

namespace spruce {	
	class Deferred_Renderer;

	class Deferred_Geometry_Pass : public Render_Pass {
	public:
		Deferred_Geometry_Pass(Deferred_Renderer* renderer);
		void render(Scene* scene) override;
	};

	class Deferred_Lighting_Pass : public Render_Pass {
	private:
		std::unique_ptr<FSQ_Mesh> fsq_mesh_;
		std::unique_ptr<OpenGL_Program> fsq_program_;

	public:
		Deferred_Lighting_Pass(Deferred_Renderer* renderer);
		void render(Scene* scene) override;
	};

	class Deferred_Renderer : public Renderer {
	friend class Deferred_Geometry_Pass;
	friend class Deferred_Lighting_Pass;

	private:
		std::unique_ptr<OpenGL_Texture> position_texture_;
		std::unique_ptr<OpenGL_Texture> normal_texture_;
		std::unique_ptr<OpenGL_Texture> albedo_texture_;
		std::unique_ptr<OpenGL_Frame_Buffer> geometry_fbo_;
		
	public:
		Deferred_Renderer(Game* game)
			: Renderer(game) { }

		bool initialize() override;
	};
}