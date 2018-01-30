#pragma once

#include "spruce_renderer.h"
#include "spruce_opengl_texture.h"
#include "spruce_mesh.h"
#include "spruce_opengl_frame_buffer.h"
#include "spruce_opengl_program.h"
#include "spruce_mesh_renderer_component.h"
#include "spruce_opengl_window.h"

namespace spruce {	

	class DeferredRenderer;

	class GeometryRenderPass : public RenderPass {
	public:
		GeometryRenderPass(DeferredRenderer& renderer);
		void renderScene(Scene& scene) override;
	};

	class LightingRenderPass : public RenderPass {
	private:
		std::unique_ptr<FSQMesh> mFSQMesh;
		std::unique_ptr<GLShaderProgram> mFSQProgram;

	public:
		LightingRenderPass(DeferredRenderer& renderer);
		void renderScene(Scene& scene) override;
	};

	class DeferredRenderer : public Renderer {
	friend class GeometryRenderPass;
	friend class LightingRenderPass;

	private:
		std::unique_ptr<GLTexture> mPositionTexture;
		std::unique_ptr<GLTexture> mNormalTexture;
		std::unique_ptr<GLTexture> mAlbedoTexture;
		std::unique_ptr<GLFramebuffer> mGeometryFBO;
		
	public:
		bool initialize(GLWindow const& window);
	};
}