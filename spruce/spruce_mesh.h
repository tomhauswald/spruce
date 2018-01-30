#pragma once

#include "spruce_opengl_vertex_array.h"
#include "spruce_opengl_array_buffer.h"
#include "spruce_opengl_element_buffer.h"
#include "spruce_vertex.h"

namespace spruce {

	template<class VertexFormat>
	class Mesh {
	private:
		// Vertices.
		std::vector<VertexFormat> mVertices;
		GLArrayBuffer mVBO;

		// Indices.
		std::vector<uint16_t> mIndices;
		GLElementBuffer mIBO;

		// Vertex array.
		GLVertexArray mVAO;

		// Initialization flag.
		bool mInitialized = false;

	protected:
		virtual void initializeVAO(GLVertexArray& vao) = 0;

	public:
		Mesh() : 
			mVBO(GLBufferUsage::StaticDraw),
			mIBO(GLBufferUsage::StaticDraw) {
		}

		void initialize() {
			mVAO.bind();
			mVBO.bind();
			mIBO.bind();
			initializeVAO(mVAO);
			glUnbindVertexArray();
			glUnbindArrayBuffer();
			glUnbindElementBuffer();
			mInitialized = true;
		}

		inline std::vector<VertexFormat>& getVertices() { return mVertices; }
		inline std::vector<VertexFormat> const& getVertices() const { return mVertices; }

		inline std::vector<uint16_t>& getIndices() { return mIndices; }
		inline std::vector<uint16_t> const& getIndices() const { return mIndices; }

		inline GLVertexArray& getVAO() { return mVAO; }

		// Upload updated vertex and index data to GPU.
		void updateMeshData() {
			panic_if(!mInitialized, "Trying to update uninitialized mesh.");

			mVBO.bind();
			mVBO.buffer(mVertices.size() * sizeof(VertexFormat), &mVertices[0]);
			glUnbindArrayBuffer();

			mIBO.bind();
			mIBO.buffer(mIndices.size() * sizeof(uint16_t), &mIndices[0]);
			glUnbindElementBuffer();
		}
	};

	class FSQMesh : public Mesh<FSQVertex> {
	public:
		virtual void initializeVAO(GLVertexArray& vao) override {
			vao.enableAttributeSlot(0);
			vao.storeFvec2Attribute(0);
		}
	};

	class TexturedMesh : public Mesh<TexturedVertex> {
	public:
		virtual void initializeVAO(GLVertexArray& vao) {
			vao.enableAttributeSlot(0);
			vao.enableAttributeSlot(1);
			vao.enableAttributeSlot(2);
			vao.enableAttributeSlot(3);

			vao.storeFvec3Attribute(0, sizeof(TexturedVertex), 0 * sizeof(fvec3)); // Position
			vao.storeFvec3Attribute(1, sizeof(TexturedVertex), 1 * sizeof(fvec3)); // Normal
			vao.storeFvec3Attribute(2, sizeof(TexturedVertex), 2 * sizeof(fvec3)); // Color
			vao.storeFvec2Attribute(3, sizeof(TexturedVertex), 3 * sizeof(fvec3)); // UV
		}
	};
}