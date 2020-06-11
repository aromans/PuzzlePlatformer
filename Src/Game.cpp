#define STB_IMAGE_IMPLEMENTATION

#include "Game.h"

#include <stdio.h>
#include <cmath>
#include <string>

Game::~Game() 
{
	glfwDestroyWindow(m_MainWindow);
	glfwTerminate();

	for (size_t i = 0; i < 1024; ++i) {
		Keys[i] = 0;
	}
}

glm::vec3 direction_of_light = glm::vec3(0.0f, 0.0f, -1.0f);

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

	m_MainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME.c_str(), nullptr, nullptr);

	if (!m_MainWindow) {
		printf("GLFW window creation failed!");
		glfwTerminate();
		return false;
	}

	// Get Buffer Size Information
	glfwGetFramebufferSize(m_MainWindow, &m_BufferWidth, &m_BufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(m_MainWindow);	
	// TODO: ^^ Apparently its possible to draw to different windows - perhaps method for the different puzzle platforms/characters?? Switching Window Context 
	// TODO: ^^ 6/08/2020 - Geometry Shader might be the better option to render each level/"dimension" . . .

	// Handle Key + Mouse Input
	CreateCallBacks();

	// Keeps cursor in the window
	glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(m_MainWindow);
		glfwTerminate();
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport Size
	glViewport(0, 0, m_BufferWidth, m_BufferHeight);

	// Assigns this window for listening to user input
	glfwSetWindowUserPointer(m_MainWindow, this);

	// Calculate Projection Matrix
	proj = glm::perspective(glm::radians(45.0f), (GLfloat)m_BufferWidth / (GLfloat)m_BufferHeight, 0.1f, 100.0f);

	return true;
}

void Game::Start() 
{
	// Mesh & Shader Initialization
	m_MeshList.push_back(CreateMesh());	 // Pyramid 1
	m_MeshList.push_back(CreateMesh());	 // Pyramid 2
	m_MeshList.push_back(CreateFloor()); // Floor
	CreateShader();

	// Camera Initialization
	m_MainCamera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	// Texture Initialization
	m_BrickTexture = Texture("Textures/brick.png");
	m_BrickTexture.LoadTexture();

	m_DirtTexture = Texture("Textures/dirt.png");
	m_DirtTexture.LoadTexture();

	// Directional Light
	m_Light = DirectionalLight(direction_of_light, glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.3f);

	// Point Lights
	PointLight p1 = PointLight(glm::vec3(-4.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 1.0f);
	PointLight p2 = PointLight(glm::vec3(4.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f, glm::vec3(0.0f, 0.0f, 1.0f), 0.4f, 0.8f);
	m_PointLights.push_back(p1);
	m_PointLights.push_back(p2);

	// Material (Specular)
	m_MaterialOne = Material(glm::vec3(1.0f));
}

void Game::HandleInput()
{
	// Get + Handle user input events
	glfwPollEvents();
}


void Game::CreateCallBacks() 
{
	glfwSetKeyCallback(m_MainWindow, HandleKeys);
	glfwSetCursorPosCallback(m_MainWindow, HandleMouse);
}

// TODO: Create a custom method for User Events -- Handling Input, Engine Wide Events, Etc . . .
void Game::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode) 
{
	Game* theGame = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			theGame->Keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			theGame->Keys[key] = false;
		}
	}
}

void Game::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Game* theGame = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (theGame->MouseFirstMoved) {
		theGame->LastPos = std::make_pair(xPos, yPos);
		theGame->MouseFirstMoved = false;
	}

	theGame->ChangedPos = std::make_pair(xPos - theGame->LastPos.first, theGame->LastPos.second - yPos);
	theGame->LastPos = std::make_pair(xPos, yPos);
}

void Game::CalculateAverageNormals(unsigned int* indices, unsigned int indexCount, GLfloat* vertices, 
								   unsigned int vertexCount, unsigned int vLength, unsigned int normalOffset) 
{
	for (size_t i = 0; i < indexCount; i += 3) 
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < vertexCount/vLength; ++i) 
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

MeshRenderer* Game::CreateMesh()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	MeshRenderer *obj = new MeshRenderer();
	obj->CreateMesh(vertices, indices, 32, 12);
	//m_MeshList.push_back(obj);
	return obj;
}

MeshRenderer* Game::CreateFloor()
{
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,   10.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,	 0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,	10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	MeshRenderer* floor = new MeshRenderer();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6);
	//m_MeshList.push_back(floor);
	return floor;
}

void Game::CreateShader() {
	Shader *shader = new Shader("Shaders/vShader.glsl", "Shaders/fShader.glsl");
	m_ShaderList.push_back(shader);
}

void Game::Update(double dt) 
{ 
	// Camera Controls 
	m_MainCamera.Move(Keys, dt);
	m_MainCamera.Rotate(GetChangedPos());
}

void Game::Render()
{
	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(0.3f, .37f, .44f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader* shader = m_ShaderList[0];

	shader->Set1i(m_PointLights.size(), "pointLightCount");

	m_Light.SendToShader(*shader);

	for (int i = 0; i < m_PointLights.size(); ++i) {
		m_PointLights[i].SendToShader(*shader, i);
	}

	m_MaterialOne.SendToShader(*shader);
	shader->SetMat4f(proj, "projection");
	shader->SetMat4f(m_MainCamera.CalculateViewMatrix(), "view");
	shader->SetVec3f(m_MainCamera.GetPosition(), "cameraPos");

	// Brick Pyramid
	glm::mat4 model(1.0f); // Identity 4x4 Matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(currSize, currSize, 1.0f));
	glm::mat4 transposedInverse = glm::transpose(glm::inverse(model));	// Its expensive doing this on the GPU, 
																		// so we do it on the CPU and pass it to the GPU
	shader->SetMat4f(model, "model");
	shader->SetMat4f(transposedInverse, "inverseTModel");
	m_BrickTexture.UseTexture();
	shader->Use();
	m_MeshList[0]->Render();

	// Dirt Pyramid
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(currSize + 0.2f, currSize + 0.2f, 1.0f));
	transposedInverse = glm::transpose(glm::inverse(model));
	shader->SetMat4f(model, "model");
	shader->SetMat4f(transposedInverse, "inverseTModel");
	m_DirtTexture.UseTexture();
	shader->Use();
	m_MeshList[1]->Render();

	// Floor Model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	transposedInverse = glm::transpose(glm::inverse(model));
	shader->SetMat4f(model, "model");
	shader->SetMat4f(transposedInverse, "inverseTModel");
	m_DirtTexture.UseTexture();
	shader->Use();
	m_MeshList[2]->Render();

	glUseProgram(0);

	glfwSwapBuffers(m_MainWindow);
}
