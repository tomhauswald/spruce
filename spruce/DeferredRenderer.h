#pragma once

#include "Renderer.h"
#include "GLTexture.h"
#include "Mesh.h"
#include "GLFramebuffer.h"
#include "GLShaderProgram.h"
#include "MeshRendererComponent.h"
#include "GLWindow.h"

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