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

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace spruce;

class Test_Scene : public Game_Object {
private:
	std::unique_ptr<OpenGL_Texture> grass_texture_;
	std::unique_ptr<Textured_Mesh> mesh_;
	OpenGL_Program* program_;

public:
	Test_Scene(Game* game, Game_Object* parent, OpenGL_Program* program)
		: Game_Object(game, parent),
		  program_(program) {

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

		// Load texture.
		grass_texture_ = std::make_unique<OpenGL_Texture>();
		grass_texture_->set_upsampling_mode(OpenGL_Sampling_Mode::Point);
		grass_texture_->set_downsampling_mode(OpenGL_Sampling_Mode::Point, false, OpenGL_Sampling_Mode::Point);
		grass_texture_->set_max_anisotropy(1.0f);
		grass_texture_->upload_bitmap_data(Bitmap { "grass.png" });
	}

	virtual void draw() {
		program_->use();
		program_->uniform("uTexture")->store(0);
		grass_texture_->bind(0);

		fmat4x4 world_view_projection;
		world_view_projection = glm::translate(world_view_projection, { 0.0f, -0.75f, 0.0f });
		world_view_projection = glm::scale(world_view_projection, { 1.0f / game_->window().aspect_ratio(), 1.0f, 1.0f });
		world_view_projection = glm::rotate(world_view_projection, pi<float>() / 4.0f, { 0.0f, 0.0f, 1.0f });
		program_->uniform("uWorldViewProjection")->store(world_view_projection);

		mesh_->draw();
	}
};

class Test_Game : public Game {
private:
	std::unique_ptr<OpenGL_Program> textured_program_;

public:
	Test_Game(OpenGL_Window_Settings const& window_settings,
			  OpenGL_Context_Settings const& context_settings)
		: Game(window_settings, context_settings) {
	}

	virtual bool initialize() {
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
			textured_program_->add_uniform("uWorldViewProjection");
			textured_program_->add_uniform("uTexture");
		}

		scene_root_->add_child(
			std::make_unique<Test_Scene>(
			    this,
			    scene_root_.get(),
			    textured_program_.get()
		    )
		);
		return true;
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

	Test_Game { ws, cs }.run();
}