#pragma once

#include "spruce_opengl_vertex_array.h"
#include "spruce_opengl_array_buffer.h"
#include "spruce_opengl_element_buffer.h"

namespace spruce {

	template<class VertexFormat>
	class Mesh {
	private:
		// Vertices.
		std::vector<VertexFormat> vertices_;
		OpenGL_Array_Buffer vbo_;

		// Indices.
		std::vector<uint16_t> indices_;
		OpenGL_Element_Buffer ibo_;

		// Vertex array.
		OpenGL_Vertex_Array vao_;

        #ifdef _DEBUG
		  // Initialization flag.
		  bool initialized_ = false;
        #endif

	protected:
		virtual void initialize_vertex_array(OpenGL_Vertex_Array& vao) = 0;

	public:
		Mesh()
			: vbo_(OpenGL_Buffer_Usage::Static_Draw),
			ibo_(OpenGL_Buffer_Usage::Static_Draw) {}

		void initialize() {
			vao_.bind();
			vbo_.bind();
			ibo_.bind();
			initialize_vertex_array(vao_);
			glUnbindVertexArray();
			glUnbindArrayBuffer();
			glUnbindElementBuffer();

			#ifdef _DEBUG
			  initialized_ = true;
			#endif
		}

		inline std::vector<VertexFormat>& vertices() { return vertices_; }
		inline std::vector<VertexFormat> const& vertices() const { return vertices_; }

		inline std::vector<uint16_t>& indices() { return indices_; }
		inline std::vector<uint16_t> const& indices() const { return indices_; }

		void draw() {

			#ifdef _DEBUG
			  panic_if(!initialized_, "Trying to draw uninitialized mesh.");
			#endif

			vao_.bind();
			glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, nullptr);
			glUnbindVertexArray();
		}

		// Upload updated vertex and index data to GPU.
		void update() {

			#ifdef _DEBUG
			  panic_if(!initialized_, "Trying to update uninitialized mesh.");
			#endif

			vbo_.bind();
			vbo_.buffer(vertices_.size() * sizeof(VertexFormat), &vertices_[0]);
			glUnbindArrayBuffer();

			ibo_.bind();
			ibo_.buffer(indices_.size() * sizeof(uint16_t), &indices_[0]);
			glUnbindElementBuffer();
		}
	};
}