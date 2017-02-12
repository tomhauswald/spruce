#include <spruce_log.h>
#include <spruce_opengl_window.h>
#include <spruce_opengl_texture.h>
#include <spruce_opengl_vertex_array.h>
#include <spruce_opengl_array_buffer.h>
#include <spruce_opengl_element_buffer.h>
#include <spruce_opengl_shader.h>
#include <spruce_opengl_program.h>
#include <spruce_mesh.h>

using namespace spruce;

int main() {
	Log::msg.register_callback([](std::string const& message) { std::cout << message; });
	Log::err.register_callback([](std::string const& message) { std::cerr << message; });

	OpenGL_Window_Settings ws;
	ws.caption = "Spruce Engine Test Application";
	ws.width = 1280;
	ws.height = 720;
	ws.doubleBufferingEnabled = true;
	ws.fullscreen = false;
	ws.maximized = false;
	ws.resizable = false;
	
	OpenGL_Context_Settings cs;
	cs.majorVersion = 4;
	cs.minorVersion = 5;
	cs.coreProfileEnabled = true;
	cs.forwardCompatibilityEnabled = true;

	OpenGL_Window window { ws, cs };

	OpenGL_Shader vs { OpenGL_Shader_Type::Vertex };
	vs.compile_from_source("basic.vert");

	OpenGL_Shader fs { OpenGL_Shader_Type::Fragment };
	fs.compile_from_source("basic.frag");

	OpenGL_Program program { "basic" };
	program.attach_shader(vs);
	program.attach_shader(fs);
	program.link();

	Mesh mesh;
	
	mesh.set_positions({
		{ -1.0f,  1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },
		{  1.0f,  1.0f, 0.0f } 
	});

	mesh.set_colors({
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
		{ 1, 1, 1 }
	});

	mesh.set_uvs({
		{ 0, 0 },
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 }
	});

	mesh.set_indices({
		0, 1, 2,
		2, 3, 0
	});

	while (!window.should_close()) {
		window.poll_events();

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program.id());
		mesh.draw();

		window.swap_buffers();
	}
}