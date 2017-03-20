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
#include <spruce_opengl_frame_buffer.h>
#include <spruce_game.h>
#include <spruce_scene.h>
#include <spruce_deferred_renderer.h>
#include <spruce_transform_component.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace spruce;

class Test_Scene : public Scene {
private:
	std::unique_ptr<OpenGL_Program> textured_program_;
	std::unique_ptr<OpenGL_Texture> grass_texture_;
	std::unique_ptr<Textured_Mesh> mesh_;

public:
	bool initialize() {

		// Load the shader program used to render geometry to the G-buffer.
		textured_program_ = std::make_unique<OpenGL_Program>("textured");
		{
			OpenGL_Shader vs { OpenGL_Shader_Type::Vertex };
			vs.compile_from_source("textured.vert");

			OpenGL_Shader fs { OpenGL_Shader_Type::Fragment };
			fs.compile_from_source("textured.frag");

			textured_program_->attach_shader(vs);
			textured_program_->attach_shader(fs);
			textured_program_->link();
			textured_program_->add_uniform("uWorldViewProjection").store(fmat4x4 { 1.0f });
			textured_program_->add_uniform("uTexture");
		}

		// Construct test mesh.
		mesh_ = std::make_unique<Textured_Mesh>();
		mesh_->initialize();
		auto& vertices = mesh_->vertices();
		vertices.push_back({
			{ -1.0f,  1.0f,  0.0f },
			{ 0.0f,  0.0f,  0.0f },
			{ 1.0f,  1.0f,  1.0f },
			{ 0.0f,  1.0f }
		});
		vertices.push_back({
			{ -1.0f, -1.0f,  0.0f },
			{ 0.0f,  0.0f,  0.0f },
			{ 1.0f,  1.0f,  1.0f },
			{ 0.0f,  0.0f }
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
		auto& indices = mesh_->indices();
		indices.insert(indices.end(), {
			0, 1, 2,
			2, 3, 0
		});
		mesh_->update();

		auto transform_ = std::make_unique<Transform_Component>();

		// Load texture.
		grass_texture_ = std::make_unique<OpenGL_Texture>();
		grass_texture_->set_upsampling_mode(OpenGL_Sampling_Mode::Point);
		grass_texture_->set_downsampling_mode(OpenGL_Sampling_Mode::Point, false, OpenGL_Sampling_Mode::Point);
		grass_texture_->set_max_anisotropy(1.0f);
		grass_texture_->upload_bitmap_data(Bitmap { "grass.png" });

		auto obj = root()->add_child("object", std::make_unique<Game_Object>());
		
		auto transform = (Transform_Component*)obj->add_component(
			"transform",
			std::make_unique<Transform_Component>()
		);
		
		transform->set_local_position(0, 0, 0);
		transform->set_local_rotation(0, 0, 0);
		transform->set_local_scale(1, 1, 1);

		auto mesh_renderer = (Textured_Mesh_Renderer_Component*) obj->add_component(
			"mesh", 
			std::make_unique<Textured_Mesh_Renderer_Component>()
		);
		
		mesh_renderer->set_program(textured_program_.get());
		mesh_renderer->set_texture(grass_texture_.get());
		mesh_renderer->set_texture_uniform_name("uTexture");
		mesh_renderer->set_mesh(mesh_.get());

		return Scene::initialize();
	}
};

class Test_Game : public Game {
public:
	Test_Game() {

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
		
		set_window(std::make_unique<OpenGL_Window>(ws, cs));

		auto renderer = std::make_unique<Deferred_Renderer>(this);
		renderer->initialize();
		set_renderer(std::move(renderer));

		add_scene("main", std::make_unique<Test_Scene>());
		set_start_scene("main");
	}
};

int main() {
	Log::msg.register_callback([](std::string const& message) { std::cout << message; });
	Log::err.register_callback([](std::string const& message) { std::cerr << message; });

#ifdef _WIN32
	// Enable message box debugging on Windows platforms.
	Log::err.register_callback([](std::string const& message) { MessageBoxA(nullptr, message.c_str(), "Error", 0); });
#endif

	Test_Game {}.run();
}