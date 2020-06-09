#include "Game.h"

#include <stdio.h>
#include <cmath>
#include <string>

Game::~Game() 
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

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
	glfwGetFramebufferSize(mainWindow, &m_BufferWidth, &m_BufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);	
	// TODO: ^^ Apparently its possible to draw to different windows - perhaps method for the different puzzle platforms/characters?? Switching Window Context 
	// TODO: ^^ 6/08/2020 - Geometry Shader might be the better option to render each level/"dimension" . . .

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport Size
	glViewport(0, 0, m_BufferWidth, m_BufferHeight);

	CreateMesh();
	CreateMesh();
	CreateShader();

	proj = glm::perspective(45.0f, (GLfloat)m_BufferWidth / (GLfloat)m_BufferHeight, 0.1f, 100.0f);

	return true;
}

void Game::Start() { }

void Game::HandleInput()
{
	// Get + Handle user input events
	glfwPollEvents();
}

void Game::CreateMesh()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	MeshRenderer *obj = new MeshRenderer();
	obj->CreateMesh(vertices, indices, 12, 12);
	m_MeshList.push_back(obj);
}

void Game::CreateShader() {
	Shader *shader = new Shader();
	shader->CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
	m_ShaderList.push_back(shader);
}


// Temporary Variables to move some pyramid meshes 
bool direction = true;
bool sizeDirection = true;
float currAngle = 0.0f;
float currSize = 0.4f;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

void Game::Update() 
{ 
	if (direction) {
		triOffset += triIncrement;
	} else {
		triOffset -= triIncrement;
	}

	if (std::abs(triOffset) >= triMaxOffset) {
		direction = !direction;
	}

	currAngle += 0.5f;
	if (currAngle >= 360) {
		currAngle -= 360;
	}
}

void Game::Render()
{
	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_ShaderList[0]->UseShader();

	GLuint uniformModel = 0, uniformProj = 0;
	uniformModel = m_ShaderList[0]->GetModelLocation();
	uniformProj = m_ShaderList[0]->GetProjectionLocation();

	glm::mat4 model(1.0f); // Identity 4x4 Matrix
	model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
	model = glm::rotate(model, glm::radians(currAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(currSize, currSize, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  // Uniform Value Setting
	glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
	m_MeshList[0]->Render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
	model = glm::rotate(model, glm::radians(currAngle), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(currSize + 0.2f, currSize + 0.2f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));  // Uniform Value Setting
	m_MeshList[1]->Render();

	glUseProgram(0);

	glfwSwapBuffers(mainWindow);
}
