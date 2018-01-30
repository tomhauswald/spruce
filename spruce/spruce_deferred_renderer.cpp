#include "spruce_deferred_renderer.h"
#include "spruce_game.h"

namespace spruce {

	GeometryRenderPass::GeometryRenderPass(DeferredRenderer& renderer) : 
		RenderPass(renderer) {
	}

	void GeometryRenderPass::renderScene(Scene& scene) {
		auto& dr = *(DeferredRenderer*) &mRenderer;

		// Prepare rendering to G-buffer.
		dr.mGeometryFBO->bind();
		dr.mGeometryFBO->clear({ 0, 0, 0 });

		// Render scene to G-buffer.
		scene.getRoot().draw();
	}

	LightingRenderPass::LightingRenderPass(DeferredRenderer& renderer) :
		RenderPass(renderer) {

		// Load the shader program used to render FSQs.
		mFSQProgram = std::make_unique<GLShaderProgram>("fsq");
		{
			GLShader vs { GLShaderType::Vertex };
			vs.compileFromSource("fsq.vert");

			GLShader fs { GLShaderType::Fragment };
			fs.compileFromSource("fsq.frag");

			mFSQProgram->addShader(vs);
			mFSQProgram->addShader(fs);
			mFSQProgram->link();
			mFSQProgram->addUniformVar("uPositionTexture").store(0);
			mFSQProgram->addUniformVar("uNormalTexture").store(1);
			mFSQProgram->addUniformVar("uAlbedoTexture").store(2);
		}

		// Construct FSQ mesh.
		mFSQMesh = std::make_unique<FSQMesh>();
		{
			mFSQMesh->initialize();
			mFSQMesh->getVertices().insert(
				mFSQMesh->getVertices().end(), {
					{ { -1.0f,  1.0f } },
					{ { -1.0f, -1.0f } },
					{ { 1.0f, -1.0f } },
					{ { 1.0f,  1.0f } }
				}
			);
			mFSQMesh->getIndices().insert(
				mFSQMesh->getIndices().end(), {
					0, 1, 2,
					2, 3, 0
				}
			);
			mFSQMesh->updateMeshData();
		}
	}

	void LightingRenderPass::renderScene(Scene& scene) {
		auto& dr = *(DeferredRenderer*) &mRenderer;

		// Restore default FBO.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw fullscreen quad.
		mFSQProgram->use();
		mFSQProgram->getUniformVar("uPositionTexture").store(0);
		mFSQProgram->getUniformVar("uNormalTexture").store(1);
		mFSQProgram->getUniformVar("uAlbedoTexture").store(2);
		
		dr.mPositionTexture->bind(0);
		dr.mNormalTexture->bind(1);
		dr.mAlbedoTexture->bind(2);

		mFSQMesh->getVAO().bind();
		glDrawElements(GL_TRIANGLES, mFSQMesh->getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
		glUnbindVertexArray();
	}

	bool DeferredRenderer::initialize(GLWindow const& window) {
		mPositionTexture = std::make_unique<GLTexture>();
		mPositionTexture->uploadBitmap(
			Bitmap {
			window.getWidth(),
			window.getHeight()
		});

		mNormalTexture = std::make_unique<GLTexture>();
		mNormalTexture->uploadBitmap(
			Bitmap {
			window.getWidth(),
			window.getHeight()
		});

		mAlbedoTexture = std::make_unique<GLTexture>();
		mAlbedoTexture->uploadBitmap(
			Bitmap {
			window.getWidth(),
			window.getHeight()
		});

		mGeometryFBO = std::make_unique<GLFramebuffer>();
		mGeometryFBO->addTextureAttachment(GL_COLOR_ATTACHMENT0, *mPositionTexture);
		mGeometryFBO->addTextureAttachment(GL_COLOR_ATTACHMENT1, *mNormalTexture);
		mGeometryFBO->addTextureAttachment(GL_COLOR_ATTACHMENT2, *mAlbedoTexture);

		mRenderPasses.push_back(std::make_unique<GeometryRenderPass>(*this));
		mRenderPasses.push_back(std::make_unique<LightingRenderPass>(*this));
		return true;
	}
}