#include "Game.h"

#include <stdio.h>

bool Game::Initialize()
{
	// Initialize GLFW
	if (!glfwInit()) {
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return false;
	}

	// Setup GLFW window properties
	// OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Allow Forwards Compatibility

	mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME.c_str(), nullptr, nullptr);

	if (!mainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return false;
	}

	// Get Buffer Size Information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);	// TODO: Apparently its possible to draw to different windows - perhaps method for the different puzzle platforms/characters?? Switching Window Context 

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return false;
	}

	// Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	return true;
}

void Game::Start() { }

void Game::HandleInput()
{
	// Get + Handle user input events
	glfwPollEvents();
}

GLuint VAO, VBO, Shader;

// Vertex Shader
static const char* vShader = 
R"(		     
		#version 460						     
									     
		layout (location = 0) in vec3 pos;	     
									     
		void main() 
		{						     
			gl_Position = vec4(0.4f * pos, 1.0f);
		}
)";

// Fragment Shader
static const char* fShader = 
R"(		 
		#version 460						 
									 
		layout (location = 0) out vec4 color;						 
									 
		void main() 
		{						 
			color = vec4(0.0, 1.0, 0.0, 1.0);
		}
)";

void Game::CreateTriangle() 
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	// Generate and Bind Vertex Array Object(s)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and Bind Vertex Buffer Object(s)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbinding Vertex Buffer Object(s)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbinding Vertex Array Object(s)
	glBindVertexArray(0);
}

void Game::AddShader(const GLuint& program, const char* shaderCode, GLenum shaderType) {
	GLuint created_shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint code_length[1];
	code_length[0] = strlen(shaderCode);

	glShaderSource(created_shader, 1, code, code_length);
	glCompileShader(created_shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(created_shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(created_shader, sizeof(eLog), nullptr, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, created_shader);
}

void Game::CompileShaders() {
	Shader = glCreateProgram();

	if (!Shader) {
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(Shader, vShader, GL_VERTEX_SHADER);
	AddShader(Shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(Shader);
	glGetProgramiv(Shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(Shader, sizeof(eLog), nullptr, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(Shader);
	glGetProgramiv(Shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(Shader, sizeof(eLog), nullptr, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

void Game::Update() { }

void Game::Render()
{
	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(Shader);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glUseProgram(0);

	glfwSwapBuffers(mainWindow);
}
