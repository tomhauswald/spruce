#pragma once

#include "spruce_opengl_vertex_array.h"
#include "spruce_opengl_array_buffer.h"
#include "spruce_opengl_element_buffer.h"

namespace spruce {
	class Mesh {
	private:
		OpenGL_Vertex_Array vao_;
		
		OpenGL_Array_Buffer position_vbo_;
		OpenGL_Array_Buffer color_vbo_;
		OpenGL_Array_Buffer uv_vbo_;

		OpenGL_Element_Buffer ibo_;
		size_t index_count_;

	public:
		Mesh() 
		: position_vbo_(OpenGL_Buffer_Usage::Static_Draw),
		  color_vbo_(OpenGL_Buffer_Usage::Static_Draw),
		  uv_vbo_(OpenGL_Buffer_Usage::Static_Draw),
		  ibo_(OpenGL_Buffer_Usage::Static_Draw),
		  index_count_(0) {

			vao_.bind();
			
			vao_.enable_attribute(0);
			position_vbo_.bind();
			vao_.store_fvec3_attribute(0);

			vao_.enable_attribute(1);
			color_vbo_.bind();
			vao_.store_fvec3_attribute(1);

			vao_.enable_attribute(2);
			uv_vbo_.bind();
			vao_.store_fvec2_attribute(2);

			ibo_.bind();

			glBindVertexArray(0);
		}

		void set_indices(std::vector<uint16_t> const& indices) {
			ibo_.bind();
			ibo_.buffer(indices.size() * sizeof(uint16_t), &indices[0]);
			index_count_ = indices.size();
		}

		void set_positions(std::vector<fvec3> const& positions) {
			position_vbo_.bind();
			position_vbo_.buffer(positions.size() * sizeof(fvec3), &positions[0]);
		}

		void set_colors(std::vector<fvec3> const& colors) {
			color_vbo_.bind();
			color_vbo_.buffer(colors.size() * sizeof(fvec3), &colors[0]);
		}

		void set_uvs(std::vector<fvec2> const& uvs) {
			uv_vbo_.bind();
			uv_vbo_.buffer(uvs.size() * sizeof(fvec3), &uvs[0]);
		}

		void draw() {
			vao_.bind();
			glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_SHORT, nullptr);
		}
	};
}