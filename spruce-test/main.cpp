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
#include <spruce_sprite_renderer_component.h>
#include <spruce_opengl_textures.h>
#include <spruce_opengl_program_manager.h>


#ifdef _WIN32
#include <Windows.h>
#endif

using namespace spruce;

void print_mat4(mat4 const& m) {
	for (auto i = 0; i < 4; ++i) {
		for (auto j = 0; j < 4; ++j) {
			std::cout << m[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print_vec3(vec3 const& v) {
	std::cout << "[ " << v.x << "\t" << v.y << "\t" << v.z << " ]" << std::endl;
}

class RotatorComponent : public Component {
private:
	float mRotationSpeed;

public:
	RotatorComponent(float rotationSpeed) :
		mRotationSpeed(rotationSpeed) {
	}

	void update(float dt) override {
		mOwner->getTransform().getRotation().y += mRotationSpeed * dt;
	}
};

class TestScene : public Scene {
private:
	GLTexture* mGrassTexture;
	std::unique_ptr<TexturedMesh> mMesh;

public:
	TestScene(Game& game) :
		Scene(game) {
	}

	bool initialize() {

		// Load the shader program used to render geometry to the G-buffer.
		GLShaderProgramManager::add("textured", std::make_unique<GLShaderProgram>("textured"));
		{
			GLShader vs { GLShaderType::Vertex };
			vs.compileFromSource("textured.vert");

			GLShader fs { GLShaderType::Fragment };
			fs.compileFromSource("textured.frag");

			GLShaderProgramManager::get("textured")->addShader(vs);
			GLShaderProgramManager::get("textured")->addShader(fs);
			GLShaderProgramManager::get("textured")->link();
			GLShaderProgramManager::get("textured")->addUniformVar("uMVP");
			GLShaderProgramManager::get("textured")->addUniformVar("uTexture");
		}

		// Construct test mesh.
		mMesh = std::make_unique<TexturedMesh>();
		mMesh->initialize();
		auto& vertices = mMesh->getVertices();

		// Back
		vertices.push_back({
			{ -0.5f, 0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ 0.5f, 0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 1, 0 }
		});

		// Right
		vertices.push_back({
			{ 0.5f, 0.5f, 0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, 0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, -0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ 0.5f, 0.5f, -0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 0 }
		});

		// Left
		vertices.push_back({
			{ -0.5f, 0.5f, -0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, -0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, 0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ -0.5f, 0.5f, 0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 1, 0 }
		});
		
		// Front
		vertices.push_back({
			{ 0.5f, 0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ -0.5f, 0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 1, 0 }
		});

		auto& indices = mMesh->getIndices();
		indices.insert(indices.end(), {
			0, 1, 2,
			2, 3, 0,
			
			4, 5, 6,
			6, 7, 4,
			
			8, 9, 10,
			10, 11, 8,
	
			12, 13, 14,
			14, 15, 12
		});
		mMesh->updateMeshData();

		mGrassTexture = &GLTextureManager::add("grass", std::make_unique<GLTexture>());
		mGrassTexture->setUpsamplingMode(GLSamplingMode::Point);
		mGrassTexture->setDownsamplingMode(GLSamplingMode::Point, false, GLSamplingMode::Point);
		mGrassTexture->setMaxAnisotropy(1.0f);
		mGrassTexture->uploadBitmap(Bitmap("grass.png"));

		auto fov = 45.0f;
		auto view = glm::lookAt(vec3(0, 0, 10), { 0, 0, 0 }, { 0, 1, 0 });
		auto projection = glm::perspective(glm::radians(fov), 16.0f / 9.0f, 0.1f, 100.0f);

		// Create test object.
		auto& obj = getRoot().addChild(std::make_unique<Entity>());
		obj.addComponent(std::make_unique<RotatorComponent>(glm::pi<float>() / 2));
		
		auto& meshRendererComponent = obj.addComponentOfType<TxrMeshRendererComponent>();
		meshRendererComponent.setShaderProgram(*GLShaderProgramManager::get("textured"));
		meshRendererComponent.setMesh(mMesh.get());
		meshRendererComponent.setTextureUniformName("uTexture");
		meshRendererComponent.setMVPUniformName("uMVP");
		meshRendererComponent.setTexture(*mGrassTexture);
		meshRendererComponent.setViewProjMat(projection * view);

		return Scene::initialize();
	}
};

class TestGame : public Game {
public:
	TestGame() {

		GLWindowSettings ws;
		ws.caption = "Spruce Engine Test Application";
		ws.width = 1280;
		ws.height = 720;
		ws.vsync = false;
		ws.doubleBuffered = true;
		ws.fullscreen = false;
		ws.maximized = false;
		ws.resizable = false;

		GLContextSettings cs;
		cs.majorVersion = 4;
		cs.minorVersion = 0;
		cs.coreProfileEnabled = true;
		cs.forwardCompatibilityEnabled = true;
		
		setWindow(std::make_unique<GLWindow>(ws, cs));

		auto renderer = std::make_unique<DeferredRenderer>();
		renderer->initialize(getWindow());
		setRenderer(std::move(renderer));

		addScene("main", std::make_unique<TestScene>(*this));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glFrontFace(GL_CCW);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
};

int main() {
	Log::msg.register_callback([](std::string const& message) { std::cout << message; });
	Log::err.register_callback([](std::string const& message) { std::cerr << message; });

#ifdef _WIN32
	// Enable message box debugging on Windows platforms.
	Log::err.register_callback([](std::string const& message) { MessageBoxA(nullptr, message.c_str(), "Error", 0); });
#endif

	TestGame().run("main");
}
