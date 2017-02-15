#include <spruce_log.h>
#include <spruce_opengl_window.h>
#include <spruce_opengl_texture.h>
#include <spruce_opengl_vertex_array.h>
#include <spruce_opengl_array_buffer.h>
#include <spruce_opengl_element_buffer.h>
#include <spruce_opengl_shader.h>
#include <spruce_opengl_program.h>
#include <spruce_mesh.h>
#include <spruce_bitmap.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace spruce;

struct Simple_Vertex {
	fvec3 position;
	fvec3 normal;
	fvec3 color;
	fvec2 uv;
};

class Simple_Mesh : public Mesh<Simple_Vertex> {
public:
	virtual void initialize_vertex_array(OpenGL_Vertex_Array& vao) {
		vao.enable_attribute(0);
		vao.enable_attribute(1);
		vao.enable_attribute(2);
		vao.enable_attribute(3);

		vao.store_fvec3_attribute(0, sizeof(Simple_Vertex), 0 * sizeof(fvec3));
		vao.store_fvec3_attribute(1, sizeof(Simple_Vertex), 1 * sizeof(fvec3));
		vao.store_fvec3_attribute(2, sizeof(Simple_Vertex), 2 * sizeof(fvec3));
		vao.store_fvec2_attribute(3, sizeof(Simple_Vertex), 3 * sizeof(fvec3));
	}
};

int main() {
	Log::msg.register_callback([](std::string const& message) { std::cout << message; });
	Log::err.register_callback([](std::string const& message) { std::cerr << message; });

#ifdef _WIN32
	// Enable message box debugging on Windows platforms.
	Log::err.register_callback([](std::string const& message) { MessageBoxA(nullptr, message.c_str(), "Error", 0); });
#endif

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
	cs.minorVersion = 0;
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
	program.add_uniform("uWorldViewProjection").store(fmat4x4 { 1.0f });
	program.add_uniform("uTexture").store(0);

	Simple_Mesh mesh;
	mesh.initialize();

	auto& vertices = mesh.vertices();

	vertices.push_back({
		{ -1.0f,  1.0f,  0.0f },
		{  0.0f,  0.0f,  0.0f },
		{  1.0f,  1.0f,  1.0f },
		{  0.0f,  1.0f }
	});

	vertices.push_back({
		{ -1.0f, -1.0f,  0.0f },
		{  0.0f,  0.0f,  0.0f },
		{  1.0f,  1.0f,  1.0f },
		{  0.0f,  0.0f }
	});

	vertices.push_back({
		{ 1.0f, -1.0f,  0.0f },
		{ 0.0f,  0.0f,  0.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f,  0.0f }
	});

	vertices.push_back({
		{ 1.0f,  1.0f,  0.0f },
		{ 0.0f,  0.0f,  0.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f }
	});

	auto& indices = mesh.indices();
	indices.insert(indices.end(), {
		0, 1, 2,
		2, 3, 0
	});

	mesh.update();

	OpenGL_Texture texture;
	texture.set_upsampling_mode(OpenGL_Sampling_Mode::Point);
	texture.set_downsampling_mode(OpenGL_Sampling_Mode::Point, false, OpenGL_Sampling_Mode::Point);
	texture.set_max_anisotropy(1.0f);
	texture.upload_bitmap_data(Bitmap { "grass.png" });

	while (!window.should_close()) {
		window.poll_events();
		window.clear_buffer({ 0.0f, 0.0f, 0.0f });

		program.use();
		texture.bind(0);
		program.uniform("uTexture")->store(0);

		fmat4x4 world_view_projection;
		world_view_projection = glm::translate(world_view_projection, { 0.0f, -0.75f, 0.0f });
		world_view_projection = glm::scale(world_view_projection, { 1.0f / window.aspect_ratio(), 1.0f, 1.0f });
		world_view_projection = glm::rotate(world_view_projection, pi<float>() / 4.0f, { 0.0f, 0.0f, 1.0f });

		program.uniform("uWorldViewProjection")->store(world_view_projection);
		
		mesh.draw();

		window.swap_buffers();
	}
}