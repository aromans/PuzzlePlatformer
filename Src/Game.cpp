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

	CreateMesh();
	CreateMesh();
	CreateShader(); 

	m_MainCamera = Camera(glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.5f);

	m_BrickTexture = Texture("Textures/brick.png");
	m_BrickTexture.LoadTexture();

	m_DirtTexture = Texture("Textures/dirt.png");
	m_DirtTexture.LoadTexture();

	m_Light = Light(glm::vec3(1.0f, 1.0f, 1.0f), 0.2f,		// Ambient
					glm::vec3(2.0f, -1.0f, -2.0f), 1.0f);   // Diffuse

	proj = glm::perspective(45.0f, (GLfloat)m_BufferWidth / (GLfloat)m_BufferHeight, 0.1f, 100.0f);

	return true;
}

void Game::Start() { }

void Game::HandleInput(double deltaTime)
{
	// Get + Handle user input events
	glfwPollEvents();

	// Camera Controls 
	m_MainCamera.Move(Keys, deltaTime);
	m_MainCamera.Rotate(GetChangedPos());
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
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] = normal.x; vertices[in0 + 1] = normal.y; vertices[in0 + 2] = normal.z;
		vertices[in1] = normal.x; vertices[in1 + 1] = normal.y; vertices[in1 + 2] = normal.z;
		vertices[in2] = normal.x; vertices[in2 + 1] = normal.y; vertices[in2 + 2] = normal.z;
	}

	for (size_t i = 0; i < vertexCount/vLength; ++i) 
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void Game::CreateMesh()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Values:
	// x, y, z | u, v | Normals [x,y,z] (averaged)
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
 		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
	};

	CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	MeshRenderer *obj = new MeshRenderer();
	obj->CreateMesh(vertices, indices, 32, 12);
	m_MeshList.push_back(obj);
}

void Game::CreateShader() {
	Shader *shader = new Shader("Shaders/vShader.glsl", "Shaders/fShader.glsl");
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

	Shader* shader = m_ShaderList[0];
	//m_ShaderList[0]->Use();

	//GLuint uniformModel = 0, uniformProj = 0, uniformView = 0;
	//uniformModel = m_ShaderList[0]->GetModelLocation();
	//uniformProj = m_ShaderList[0]->GetProjectionLocation();
	//uniformView = m_ShaderList[0]->GetViewLocation();

	//GLuint uniformColor = m_ShaderList[0]->GetColorLocation();
	//GLuint uniformAmbientIntensity = m_ShaderList[0]->GetIntensityLocation();

	//GLuint uniformDirection = m_ShaderList[0]->GetDirectionLocation();
	//GLuint uniformDiffuseIntensity = m_ShaderList[0]->GetDiffuseIntensityLocation();

	m_Light.UseLight(*shader);

	glm::mat4 model(1.0f); // Identity 4x4 Matrix
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(currSize, currSize, 1.0f));

	shader->SetMat4f(model, "model");
	shader->SetMat4f(proj, "projection");
	shader->SetMat4f(m_MainCamera.CalculateViewMatrix(), "view");

	m_BrickTexture.UseTexture();

	shader->Use();

	m_MeshList[0]->Render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(currSize + 0.2f, currSize + 0.2f, 1.0f));

	shader->SetMat4f(model, "model");

	m_DirtTexture.UseTexture();

	shader->Use();

	m_MeshList[1]->Render();

	glUseProgram(0);

	glfwSwapBuffers(m_MainWindow);
}
