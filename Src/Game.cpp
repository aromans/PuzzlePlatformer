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

	// Delete Meshes
	delete m_DirtCube;
	delete m_Tree;
	delete m_Robo;

	// Delete Materials
	delete m_DirtMat;
	delete m_BrickMat;
	delete m_DirtCubeMat;
	delete m_TreeMat;
	delete m_RoboMat;

	// Delete MeshRenderers
	for (size_t i = 0; i < m_MeshList.size(); ++i) {
		delete m_MeshList[i];
	}	
	
	for (size_t i = 0; i < m_ShaderList.size(); ++i) {
		delete m_ShaderList[i];
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
	//glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		printf("GLEW Initialization failed!");
		glfwDestroyWindow(m_MainWindow);
		glfwTerminate();
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glEnable(GL_BACK)

	// Setup Viewport Size
	glViewport(0, 0, m_BufferWidth, m_BufferHeight);

	// Assigns this window for listening to user input
	glfwSetWindowUserPointer(m_MainWindow, this);

	// Calculate Projection Matrix
	GLfloat aspectRatio = (GLfloat)m_BufferWidth / (GLfloat)m_BufferHeight;
	proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	ortho = glm::ortho(-aspectRatio, aspectRatio, -20.0f, 20.0f, 0.01f, 100.0f);
	/*proj = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.0f, 1000.0f);
	proj = glm::scale(proj, glm::vec3(0.55f, 0.55f, 0.55f));*/

	return true;
}

std::vector<int> tree_locations;

void Game::Start() 
{
	// Shader Initialization
	Shader* basicShader = new Shader("Shaders/vShader.glsl", "Shaders/fShader.glsl");
	m_ShaderList.push_back(basicShader);
	Shader* normalShader = new Shader("Shaders/normalVShader.glsl", "Shaders/normalFShader.glsl");
	m_ShaderList.push_back(normalShader);

	m_DirectionalShadowShader = Shader("Shaders/vDirectionalShadowMap.glsl", "Shaders/fDirectionalShadowMap.glsl");

	// Texture Initialization
	m_DirtMat = new Material(m_ShaderList[1], "Textures/brick.png", "");
	m_DirtMat->SetProperties(glm::vec3(0.0f), 0.0f);

	m_BrickMat = new Material(m_ShaderList[1], "Textures/dirt.png", "");
	m_BrickMat->SetProperties(glm::vec3(0.3f), 0.2f);

	// Material Initialization
	m_DirtCubeMat = new Material(m_ShaderList[1], "Textures/cube.png", "Textures/cube_normal.png");
	m_DirtCubeMat->SetProperties(glm::vec3(0.0f), 0.0f);

	m_TreeMat = new Material(m_ShaderList[1], "Textures/CartoonTree.png", "");
	m_TreeMat->SetProperties(glm::vec3(0.1f), 0.1f);

	m_PineTreeLeavesMat = new Material(m_ShaderList[1], "Textures/leafs_d.png", "");
	m_PineTreeLeavesMat->SetProperties(glm::vec3(0.1f), 0.1f);

	m_PineTreeBarkMat = new Material(m_ShaderList[1], "Textures/bark_d.png", "");
	m_PineTreeBarkMat->SetProperties(glm::vec3(0.1f), 0.1f);

	m_RoboMat = new Material(m_ShaderList[1], "Textures/Robo.png", "Textures/Robo_normal.png");
	m_RoboMat->SetProperties(glm::vec3(.8f), 1.0f);

	// Primitive Mesh Initialization
	m_MeshList.push_back(CreateMesh());	 // Pyramid 1
	m_MeshList.push_back(CreateMesh());	 // Pyramid 2
	m_MeshList.push_back(CreateFloor()); // Floor

	// Load OBJ Files
	auto DirtCubeObj = LoadOBJ("ObjFiles/DirtCube.obj");
	m_DirtCube = new Mesh(std::get<0>(DirtCubeObj), std::get<1>(DirtCubeObj));

	auto TreeObj = LoadOBJ("ObjFiles/CartoonTree1.obj");
	m_Tree = new Mesh(std::get<0>(TreeObj), std::get<1>(TreeObj));

	auto PineTreeLeavesObj = LoadOBJ("ObjFiles/PineTree_Leaves.obj");
	auto PineTreeBarkObj = LoadOBJ("ObjFiles/PineTree_Bark.obj");
	m_PineTree_Leaves = new Mesh(std::get<0>(PineTreeLeavesObj), std::get<1>(PineTreeLeavesObj));
	m_PineTree_Bark = new Mesh(std::get<0>(PineTreeBarkObj), std::get<1>(PineTreeBarkObj));

	auto RoboObj = LoadOBJ("ObjFiles/TempChar.obj");
	m_Robo = new Mesh(std::get<0>(RoboObj), std::get<1>(RoboObj));

	MeshRenderer* obj = new MeshRenderer();
	obj->CreateMesh(m_DirtCube, m_DirtCubeMat);
	
	MeshRenderer* obj2 = new MeshRenderer();
	obj2->CreateMesh(m_PineTree_Leaves, m_PineTreeLeavesMat);
	MeshRenderer* obj2_1 = new MeshRenderer();
	obj2_1->CreateMesh(m_PineTree_Bark, m_PineTreeBarkMat);

	MeshRenderer* obj3 = new MeshRenderer();
	obj3->CreateMesh(m_Robo, m_RoboMat);

	m_MeshList.push_back(obj2);
	m_MeshList.push_back(obj2_1);
	m_MeshList.push_back(obj3);

	// 3x3 Level
	for (int i = 0; i < 9; ++i)
		m_MeshList.push_back(obj);

	// 5x5 Level 
	for (int i = 0; i < 25; ++i)
		m_MeshList.push_back(obj);
	tree_locations = { 4, 6, 7, 8, 12, 16, 17 };

	// Camera Initialization
	m_MainCamera = Camera(glm::vec3(0.0f, 2.f, 4.f), glm::vec3(0.0f, -1.f, -1.1f));

	// Directional Light
	m_Light = DirectionalLight(glm::vec3(0.0f, 1.f, 1.f), 2048, 2048, glm::vec3(1.0f, 1.0f, 1.0f), .3f, 1.0f);

	// Point Lights
	PointLight p1 = PointLight(glm::vec3(-1.1f, -2.f, 0.0f), 0.3f, 0.2f, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, .6f);
	PointLight p2 = PointLight(glm::vec3(1.1f, -2.f, 0.0f), 0.3f, 0.2f, 0.1f, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 2.5f);
	PointLight p3 = PointLight(glm::vec3(-1.1f, -2.f, -2.2f), 0.3f, 0.2f, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.2f);
	m_PointLights.push_back(p1);
	m_PointLights.push_back(p2);
	m_PointLights.push_back(p3);

	SpotLight s1 = SpotLight(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f, glm::vec3(1.1f, -2.f, 0.0f), 1.0f, 0.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f);
	SpotLight s2 = SpotLight(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f, glm::vec3(-1.1f, -2.0f, -2.2f), 1.0f, 0.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.5f);
	m_SpotLights.push_back(s1);
	m_SpotLights.push_back(s2);

	// Material (Specular)
	//m_MaterialOne = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(.5f), glm::vec3(1.f, 1.f, 1.f));
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
	glfwSetMouseButtonCallback(m_MainWindow, HandleMouseButton);
	glfwSetScrollCallback(m_MainWindow, HandleMouseScroll);
}

void Game::DirectionalShadowMapPass(DirectionalLight* light, Shader* shader)
{
	m_DirectionalShadowShader.Use();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	m_DirectionalShadowShader.SetMat4f(light->CalculateLightTransform(), "directionalLightTransform");

	RenderScene(&m_DirectionalShadowShader, true);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::RenderScene(Shader* shader, bool pass)
{	
	glm::mat4 model(1.0f); // Identity 4x4 Matrix
	glm::mat4 view = m_MainCamera.CalculateViewMatrix();
	glm::mat4 mvp;

	// Brick Pyramid
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	////model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	////model = glm::scale(model, glm::vec3(currSize, currSize, 1.0f));
	//transposedInverse = glm::transpose(glm::inverse(model));	// Its expensive doing this on the GPU, 
	//														    // so we do it on the CPU and pass it to the GPU
	//shader->SetMat4f(model, "model");
	//shader->SetMat4f(transposedInverse, "inverseTModel");
	//m_BrickTexture.UseTexture();
	//shader->Use();
	//m_MeshList[0]->Render();
	// Dirt Pyramid
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	////model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	////model = glm::scale(model, glm::vec3(currSize + 0.2f, currSize + 0.2f, 1.0f));
	//transposedInverse = glm::transpose(glm::inverse(model));
	//shader->SetMat4f(model, "model");
	//shader->SetMat4f(transposedInverse, "inverseTModel");
	//m_DirtTexture.UseTexture();
	//shader->Use();
	//m_MeshList[1]->Render();
	// Floor Model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	shader->SetMat4f(model, "M");
	mvp = proj * view * model;
	shader->SetMat4f(mvp, "MVP");
	m_MeshList[2]->Render(pass);

	int start = 6;
	float increment = -1.1f;
	float size = -1.1;
	float z = 0.0f;

	// Character Model Render for 5x5 level
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.1, -1.5f, z));		
	shader->SetMat4f(model, "M");
	mvp = proj * view * model;
	shader->SetMat4f(mvp, "MVP");
	m_MeshList[5]->Render(pass);

	// 5x5 Level Render
	//for (int i = 0; i < 25; ++i) {
	//	model = glm::mat4(1.0f);
	//	model = glm::translate(model, glm::vec3(size, -1.5f, z));
	//	mvp = proj * view * model;
	//	shader->SetMat4f(mvp, "MVP");
	//	m_MeshList[i + start]->Render();
	//	if (std::find(tree_locations.cbegin(), tree_locations.cend(), i) != tree_locations.cend()) {
	//		model = glm::mat4(1.0f);
	//		model = glm::translate(model, glm::vec3(size, -1.5f, z));
	//		mvp = proj * view * model;
	//		shader->SetMat4f(mvp, "MVP");
	//		m_MeshList[3]->Render();
	//	}
	//	size -= increment;
	//	if ((i + 1) % 5 == 0) {
	//		size = -2.2;
	//		z -= 1.1f;
	//	}
	//}

	for (int i = 0; i < 9; ++i) {
		// Transform the Model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(size, -1.5f, z));

		shader->SetMat4f(model, "M");
		mvp = proj * view * model;
		shader->SetMat4f(mvp, "MVP");

		// Render the Mesh
		m_MeshList[i + start]->Render(pass);

		// Level Layout Logic
		size -= increment;
		if ((i + 1) % 3 == 0) {
			size = increment;
			z -= 1.1f;
		}
	}

	// Loading the Tree Model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.1f, -1.5f, -2.2f));

	shader->SetMat4f(model, "M");
	mvp = proj * view * model;
	shader->SetMat4f(mvp, "MVP");

	m_MeshList[3]->Render(pass);
	m_MeshList[4]->Render(pass);
}

void Game::RenderPass(Shader* shader)
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Clear window
	glClearColor(0.15f, 0.11f, 0.19f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Light.SendToShader(*shader);

	shader->Set1i(m_PointLights.size(), "pointLightCount");
	for (int i = 0; i < m_PointLights.size(); ++i) {
		m_PointLights[i].SendToShader(*shader, i);

		glm::vec3 lightPosEyeSpace = m_MainCamera.CalculateViewMatrix() * glm::vec4(m_PointLights[i].GetPosition(), 1.0);
		shader->SetVec3f(lightPosEyeSpace, ("LightPositionsEyeSpace[" + std::to_string(i) + "]").c_str());
	}

	shader->Set1i(m_SpotLights.size(), "spotLightCount");
	for (int i = 0; i < m_SpotLights.size(); ++i) {
		m_SpotLights[i].SendToShader(*shader, i);
	}

	shader->SetMat4f(m_MainCamera.CalculateViewMatrix(), "V");
	shader->SetVec3f(m_MainCamera.GetPosition(), "cameraPos");
	shader->SetMat4f(m_Light.CalculateLightTransform(), "directionalLightTransform");

	m_Light.GetShadowMap()->Read(GL_TEXTURE0 + 2);
	shader->Set1i(2, "directionalShadowMap");

	RenderScene(shader, false);
}

// TODO: Create a custom method for User Events -- Handling Input, Engine Wide Events, Etc . . .
void Game::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode) 
{
	Game* theGame = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (theGame->Keys[key] == false && action == GLFW_PRESS) {
			theGame->Keys[key] = true;
		}
		else if (theGame->Keys[key] == true && action == GLFW_RELEASE) {
			theGame->Keys[key] = false;
		}
	}
}

void Game::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Game* theGame = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (theGame->MouseFirstMoved) {
		theGame->LastPos = glm::vec2(xPos, yPos);
		theGame->MouseFirstMoved = false;
	}

	theGame->MouseDelta = glm::vec2((GLfloat)(xPos - theGame->LastPos.x), (GLfloat)(theGame->LastPos.y - yPos));
	theGame->LastPos = glm::vec2(xPos, yPos);
}

void Game::HandleMouseScroll(GLFWwindow* window, double xPos, double yPos)
{
	Game* theGame = static_cast<Game*>(glfwGetWindowUserPointer(window));

	theGame->m_MainCamera.OnMouseScroll(window, xPos, yPos);
}

void Game::HandleMouseButton(GLFWwindow* window, int key, int action, int mods)
{
	Game* theGame = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		theGame->Keys[key] = true;
	}
	else {
		theGame->Keys[key] = false;
	}
}

// Utility Function for below Primitives (Pyramid)
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

// Primitives -- Pyramid && Quad 
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
	obj->CreateMesh(vertices, indices, 32, 12, m_BrickMat);
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
		-10.0f, 0.0f, -10.0f,	 0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,   10.0f,  0.0f,	0.0f, 1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,	 0.0f, 10.0f,	0.0f, 1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,	10.0f, 10.0f,	0.0f, 1.0f, 0.0f
	};

	MeshRenderer* floor = new MeshRenderer();
	floor->CreateMesh(floorVertices, floorIndices, 32, 6, m_DirtMat);
	//m_MeshList.push_back(floor);
	return floor;
}

// Update Loop
void Game::Update(double dt) 
{ 
	// Camera Controls 
	m_MainCamera.Move(Keys, dt);
	m_MainCamera.OnMouseMove(Keys, MouseDelta, LastPos, dt);
}

// Render Loop
void Game::Render()
{
	Shader* shader = m_ShaderList[1];

	DirectionalShadowMapPass(&m_Light, shader);
	RenderPass(shader);

	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glfwSwapBuffers(m_MainWindow);
}
