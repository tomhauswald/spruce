#include <spruce_log.h>
#include <spruce_opengl_window.h>
#include <spruce_opengl_texture.h>
#include <spruce_opengl_vertex_array.h>
#include <spruce_opengl_array_buffer.h>
#include <spruce_opengl_element_buffer.h>
#include <spruce_opengl_shader.h>
#include <spruce_opengl_program.h>

using namespace spruce;

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	OpenGL_Shader vs { OpenGL_Shader_Type::Vertex };
	OpenGL_Shader fs { OpenGL_Shader_Type::Fragment };

	// Create the shaders
	GLuint VertexShaderID = vs.id();
	GLuint FragmentShaderID = fs.id();

	Log::msg << VertexShaderID << " " << FragmentShaderID << "\n";

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

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

	// Setup vertex buffer.
	fvec3 vertices[] = {
		{ -1.0f, -1.0f, 0.0f },
		{  1.0f, -1.0f, 0.0f },
		{  0.0f,  1.0f, 0.0f } 
	};

	OpenGL_Array_Buffer vbo(OpenGL_Buffer_Usage::Static_Draw);
	vbo.bind();
	vbo.buffer(sizeof(vertices), vertices);

	// Setup index buffer.
	uint16_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	OpenGL_Element_Buffer ibo(OpenGL_Buffer_Usage::Static_Draw);
	ibo.bind();
	ibo.buffer(sizeof(indices), indices);

	// Setup vao.
	OpenGL_Vertex_Array vao;
	vao.bind();

	// Store vertices in attribute 0.
	vao.enable_attribute(0);
	vbo.bind();
	vao.store_fvec3_attribute(0);

	OpenGL_Shader vs { OpenGL_Shader_Type::Vertex };
	vs.compile_from_source("basic.vert");
	Log::msg << vs.id() << "\n";

	OpenGL_Shader fs { OpenGL_Shader_Type::Fragment };
	fs.compile_from_source("basic.frag");
	Log::msg << fs.id() << "\n";

	OpenGL_Program program;
	program.attach_shader(vs);
	program.attach_shader(fs);
	program.link();
	Log::msg << program.id() << "\n";

	// auto program_id = LoadShaders("basic.vert", "basic.frag");

	while (!window.should_close()) {
		window.poll_events();

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program.id());
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.swap_buffers();
	}
}