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
#include <spruce_transform.h>
#include <spruce_textured_mesh_renderer_component.h>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace spruce;

class Demo_Component : public Component {
private:
	float rotation_speed_;
public:
	Demo_Component(float rotation_speed) :
		rotation_speed_ { rotation_speed } {
	}

	void update(float dt) override {
		owner_->transform()->set_local_yaw(owner_->transform()->local_yaw() + rotation_speed_ * dt);
		Component::update(dt);
	}
};

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
			textured_program_->add_uniform("uModel").store(fmat4x4 { 1.0f });
			textured_program_->add_uniform("uView").store(fmat4x4 { 1.0f });
			textured_program_->add_uniform("uProjection").store(fmat4x4 { 1.0f });
			textured_program_->add_uniform("uTexture");
		}

		// Construct test mesh.
		mesh_ = std::make_unique<Textured_Mesh>();
		mesh_->initialize();
		auto& vertices = mesh_->vertices();
		
		// Front
		vertices.push_back({
			{ -1, -1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 0 }
		});
		vertices.push_back({
			{ 1, -1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ -1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 1, 1, 1 },
			{ 0, 1 }
		});

		auto& indices = mesh_->indices();
		indices.insert(indices.end(), {
			// front
			0, 1, 2,
			1, 3, 2
		});
		mesh_->update();

		// Load texture.
		grass_texture_ = std::make_unique<OpenGL_Texture>();
		grass_texture_->set_upsampling_mode(OpenGL_Sampling_Mode::Point);
		grass_texture_->set_downsampling_mode(OpenGL_Sampling_Mode::Point, false, OpenGL_Sampling_Mode::Point);
		grass_texture_->set_max_anisotropy(1.0f);
		grass_texture_->upload_bitmap_data(Bitmap { "grass.png" });

		// Create test object.
		auto obj = root()->add_child("object", std::make_unique<Game_Object>());

		auto obj_renderer = (Textured_Mesh_Renderer_Component*) obj->add_component(
			"mesh", 
			std::make_unique<Textured_Mesh_Renderer_Component>()
		);
		obj_renderer->set_program(textured_program_.get());
		obj_renderer->set_mesh(mesh_.get());
		obj_renderer->set_texture_uniform_name("uTexture");
		obj_renderer->set_texture(grass_texture_.get());

		auto view = glm::lookAt(vec3 { 3, 3, 3 }, { 0, 0, 0 }, { 0, 1, 0 });
		obj_renderer->set_view_matrix(view);

		auto fov = 45.0f;
		auto projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, 0.1f, 100.0f);
		obj_renderer->set_projection_matrix(projection);

		auto obj_rotator = (Demo_Component*)obj->add_component("demo", std::make_unique<Demo_Component>(0));

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

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
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