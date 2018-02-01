#include <Game.h>
#include <GLShaderProgramManager.h>
#include <GLTextureManager.h>
#include <TexturedMeshRendererComponent.h>
#include <DeferredRenderer.h>
#include <RoamingComponent.h>

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

class FPSCounterComponent : public Component {
private:
	uint32_t mFrames;
	float mTime;
	float mPeriod;

public:
	FPSCounterComponent(float period = 1.0f) :
		mFrames(0),
		mTime(0.0f),
		mPeriod(period) {
	}

	void update(float dt) override {
		mFrames++;
		if ((mTime += dt) >= mPeriod) {
			auto fps = mFrames / mTime;
			Log::msg.printf("%0.2f FPS (%5.2f ms.)\n", fps, 1000.0f / fps);
			mFrames = 0;
			mTime = 0.0f;
		}
	}
};

class TestScene : public Scene {
private:
	GLTexture* mGrassTexture;
	std::unique_ptr<TexturedMesh> mMesh;

	std::unique_ptr<TexturedMesh> createCubeMesh() {
		
		auto mesh = std::make_unique<TexturedMesh>();
		mesh->initialize();
		auto& vertices = mesh->getVertices();

		// Front
		vertices.push_back({
			{ -0.5f, 0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ 0.5f, 0.5f, -0.5f },
			{ 0, 0, -1 },
			{ 1, 1, 0 },
			{ 1, 0 }
		});

		// Back
		vertices.push_back({
			{ 0.5f, 0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ -0.5f, 0.5f, 0.5f },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 1, 0 }
		});

		// Left
		vertices.push_back({
			{ -0.5f, 0.5f, 0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, 0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, -0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ -0.5f, 0.5f, -0.5f },
			{ -1, 0, 0 },
			{ 0, 1, 1 },
			{ 1, 0 }
		});

		// Right
		vertices.push_back({
			{ 0.5f, 0.5f, -0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0},
			{ 0, 0 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, -0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, 0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ 0.5f, 0.5f, 0.5f },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 0 }
		});

		// Top
		vertices.push_back({
			{ -0.5f, 0.5f, 0.5f },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ -0.5f, 0.5f, -0.5f },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ 0.5f, 0.5f, -0.5f },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ 0.5f, 0.5f, 0.5f },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 1, 0 }
		});

		// Bottom
		vertices.push_back({
			{ -0.5f, -0.5f, -0.5f },
			{ 0, -1, 0 },
			{ 1, 0, 1 },
			{ 0, 0 }
		});
		vertices.push_back({
			{ -0.5f, -0.5f, 0.5f },
			{ 0, -1, 0 },
			{ 1, 0, 1 },
			{ 0, 1 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, 0.5f },
			{ 0, -1, 0 },
			{ 1, 0, 1 },
			{ 1, 1 }
		});
		vertices.push_back({
			{ 0.5f, -0.5f, -0.5f },
			{ 0, -1, 0 },
			{ 1, 0, 1 },
			{ 1, 0 }
		});

		auto& indices = mesh->getIndices();
		indices.insert(indices.end(), {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,
			
			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		});

		mesh->updateMeshData();
		return mesh;
	}
	
	std::unique_ptr<TexturedMesh> createGridMesh() {
		
		auto mesh = std::make_unique<TexturedMesh>();
		mesh->initialize();
		auto& vertices = mesh->getVertices();
		auto& indices = mesh->getIndices();

		uint16_t nX = 10, nZ = 10;
		for (uint16_t z = 0; z < nZ; ++z) {
			for (uint16_t x = 0; x < nX; ++x) {
				vertices.push_back({
					{ x, 0, z },
					{ 0, 1, 0 },
					{ 1, 1, 1 },
					{ x/(float)nX, z/(float)nZ }
				});
			}
		}

		auto nQuads = (nX - 1) * (nZ - 1);
		for (int i = 0; i < 10; ++i) {
			indices.push_back(i + nX);
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i + nX + 1);
			indices.push_back(i + nX);
		}

		mesh->updateMeshData();
		return mesh;
	}
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

		// Camera object.
		auto& cam = getRoot().addChild(std::make_unique<Entity>("camera"));
		cam.addComponent(std::make_unique<CameraComponent>(16.0f / 9.0f, 45.0f));
		cam.addComponent(std::make_unique<RoamingComponent>(10.0f, 90.0f));
		cam.getTransform().setPosition({ 0, 5, -5 });

		// Construct test mesh.
		mMesh = createGridMesh();

		// Load grass texture.
		mGrassTexture = &GLTextureManager::add("grass", std::make_unique<GLTexture>());
		mGrassTexture->setUpsamplingMode(GLSamplingMode::Point);
		mGrassTexture->setDownsamplingMode(GLSamplingMode::Point, false, GLSamplingMode::Point);
		mGrassTexture->setMaxAnisotropy(1.0f);
		mGrassTexture->uploadBitmap(Bitmap("grass.png"));

		// Create FPS counter.
		getRoot().addComponent(std::make_unique<FPSCounterComponent>(5.0f));

		// Create cube.
		auto& obj = getRoot().addChild(std::make_unique<Entity>());

		auto& meshRendererComponent = obj.addComponentOfType<TxrMeshRendererComponent>();
		meshRendererComponent.setShaderProgram(*GLShaderProgramManager::get("textured"));
		meshRendererComponent.setMesh(mMesh.get());
		meshRendererComponent.setTextureUniformName("uTexture");
		meshRendererComponent.setMVPUniformName("uMVP");
		meshRendererComponent.setTexture(*mGrassTexture);
		meshRendererComponent.setCamera(cam.getComponentOfType<CameraComponent>());

		return Scene::initialize();
	}
};

class TestGame : public Game {
public:
	TestGame() {

		GLWindowSettings ws;
		ws.caption = "Spruce Engine Test Application";
		ws.width = 1440;
		ws.height = 900;
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
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
};

int main() {
	Log::msg.registerCallback([](std::string const& message) { std::cout << message; });
	Log::err.registerCallback([](std::string const& message) { std::cerr << message; });

#ifdef _WIN32
	// Enable message box debugging on Windows platforms.
	Log::err.registerCallback([](std::string const& message) { MessageBoxA(nullptr, message.c_str(), "Error", 0); });
#endif

	TestGame().run("main");
}
