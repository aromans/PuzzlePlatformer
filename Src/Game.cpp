#define STB_IMAGE_IMPLEMENTATION

#include "Game.h"

#include "Renderer/Renderer.h"

#include ".\Animation\DaeLoader.h"
#include ".\Animation\ColladaImporter.h"

#include <stdio.h>
#include <cmath>
#include <string>

namespace Engine {

	Game::~Game()
	{
		glfwDestroyWindow(m_MainWindow);
		glfwTerminate();

		for (size_t i = 0; i < 1024; ++i) {
			Keys[i] = 0;
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
		glEnable(GL_CULL_FACE);
		//glEnable(GL_BLEND);

		// Setup Viewport Size
		glViewport(0, 0, m_BufferWidth, m_BufferHeight);

		// Assigns this window for listening to user input
		glfwSetWindowUserPointer(m_MainWindow, this);

		// Calculate Projection Matrix
		GLfloat aspectRatio = (GLfloat)m_BufferWidth / (GLfloat)m_BufferHeight;
		proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f);
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

		//m_MeshList.push_back(CreateFloor()); // Floor

		m_Models.push_back(new Model("ObjFiles/DirtCube.obj", "Textures/cube.png", "Textures/cube_normal.png", m_ShaderList[1]));
		m_Models.push_back(new Model("ObjFiles/PineTree_Leaves.obj", "Textures/leafs_d.png", "", m_ShaderList[1]));
		m_Models.push_back(new Model("ObjFiles/PineTree_Bark.obj", "Textures/bark_d.png", "", m_ShaderList[1]));

		//ImportColladaFile("ObjFiles/Slimer-Look.dae");

		toon = new AnimationModel("ObjFiles/Slimer-Look2.dae", "Textures/AlbedoSlimer.png", "Textures/NormalSlimer.png", m_ShaderList[1]);

		lvl_height = 3; lvl_width = 3;
		tree_locations = { 8 };// 4, 6, 7, 8, 12, 16, 17 };
		
		float x = lvl_width % 2 == 0 ? -0.5f : 0.0f;
		float z = -(lvl_height / 2) - (0.1f * (lvl_height / 2));
		z = lvl_height % 2 == 0 ? z + 0.5f : z;

		glm::vec3 cameraTarget = glm::vec3(x, -1.f, z);

		printf("For a %i x %i grid - the camera target is at (%f, %f, %f)\n", lvl_width, lvl_height, cameraTarget.x, cameraTarget.y, cameraTarget.z);

		// Camera Initialization
		m_MainCamera = Camera(glm::vec3(0.0f, 2.f, 4.f), cameraTarget);

		// Directional Light
		m_Light = DirectionalLight(glm::vec3(2.0f, 1.f, 2.f), 2048, 2048, glm::vec3(1.0f, 1.0f, 1.0f), .3f, 1.0f);

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
		m_DirectionalShadowShader.Bind();

		glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

		light->GetShadowMap()->Write();

		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

		m_DirectionalShadowShader.SetMat4f(light->CalculateLightTransform(), "directionalLightTransform");

		RenderScene(&m_DirectionalShadowShader, true);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Game::RenderScene(Shader* shader, bool pass)
	{
		glm::mat4 model(1.0f); // Identity 4x4 Matrix
		glm::mat4 view = m_MainCamera.CalculateViewMatrix();
		glm::mat4 mvp;

		// Floor Model - Keeping for now...
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		//mvp = proj * view * model;
		//shader->SetMat4f(model, "M");
		//shader->SetMat4f(glm::transpose(glm::inverse(model)), "NormalMatrix");
		//shader->SetMat4f(mvp, "MVP");
		//m_MeshList[2]->Render(pass);

		int start = 6;
		float increment = -1.1f;
		float size = -(lvl_width / 2) - (0.1f * (lvl_width / 2));
		float strt_size = size;
		float z = 0.0f;

		// Character Model Render for 5x5 level
		//model = glm::mat4(1.0f);
		////model = glm::translate(model, glm::vec3(1.1, -1.f, z));
		//model = glm::scale(model, glm::vec3(.01f, .01f, .01f));
		//mvp = proj * view * model;
		//shader->SetMat4f(model, "M");
		//shader->SetMat4f(glm::transpose(glm::inverse(model)), "NormalMatrix");
		//shader->SetMat4f(mvp, "MVP");
		toon->Render(pass, proj, view);

		// 5x5 Level Render
		for (int i = 0; i < lvl_height * lvl_width; ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(size, -1.5f, z));
			mvp = proj * view * model;
			shader->SetMat4f(model, "M");
			shader->SetMat4f(glm::transpose(glm::inverse(model)), "NormalMatrix");
			shader->SetMat4f(mvp, "MVP");
			m_Models[0]->Render(pass);

			if (std::find(tree_locations.cbegin(), tree_locations.cend(), i) != tree_locations.cend()) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(size, -1.5f, z));
				mvp = proj * view * model;

				shader->SetMat4f(model, "M");
				shader->SetMat4f(glm::transpose(glm::inverse(model)), "NormalMatrix");
				shader->SetMat4f(mvp, "MVP");

				m_Models[1]->Render(pass);
				m_Models[2]->Render(pass);
			}

			size -= increment;
			if ((i + 1) % lvl_width == 0) {
				size = strt_size;
				z -= 1.1f;
			}
		}
	}

	void Game::RenderPass(Shader* shader)
	{
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		// Clear window
		glClearColor(0.15f, 0.11f, 0.19f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);

		m_Light.SendToShader(*shader);

		shader->Set1i(m_PointLights.size(), "pointLightCount");
		for (size_t i = 0; i < m_PointLights.size(); ++i) {
			m_PointLights[i].SendToShader(*shader, i);

			glm::vec3 lightPosEyeSpace = m_MainCamera.CalculateViewMatrix() * glm::vec4(m_PointLights[i].GetPosition(), 1.0);
			shader->SetVec3f(lightPosEyeSpace, ("LightPositionsEyeSpace[" + std::to_string(i) + "]").c_str());
		}

		shader->Set1i(m_SpotLights.size(), "spotLightCount");
		for (size_t i = 0; i < m_SpotLights.size(); ++i) {
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

	//MeshRenderer* Game::CreateFloor()
	//{
	//	unsigned int floorIndices[] = {
	//		0, 2, 1,
	//		1, 2, 3
	//	};
	//	GLfloat floorVertices[] = {
	//		-10.0f, 0.0f, -10.0f,	 0.0f,  0.0f,	0.0f, 1.0f, 0.0f,
	//		 10.0f, 0.0f, -10.0f,   10.0f,  0.0f,	0.0f, 1.0f, 0.0f,
	//		-10.0f, 0.0f,  10.0f,	 0.0f, 10.0f,	0.0f, 1.0f, 0.0f,
	//		 10.0f, 0.0f,  10.0f,	10.0f, 10.0f,	0.0f, 1.0f, 0.0f
	//	};
	//	MeshRenderer* floor = new MeshRenderer();
	//	floor->CreateMesh(floorVertices, floorIndices, 32, 6, m_DirtMat);
	//	//m_MeshList.push_back(floor);
	//	return floor;
	//}

	// Update Loop
	void Game::Update(double dt)
	{
		toon->Update(dt);

		// Camera Controls 
		m_MainCamera.Move(Keys, dt);
		m_MainCamera.OnMouseMove(Keys, MouseDelta, LastPos, dt);
	}

	// Render Loop
	void Game::Render()
	{
		Shader* shader = m_ShaderList[1];

		Renderer::BeginScene();
		{
			DirectionalShadowMapPass(&m_Light, shader);
			RenderPass(shader);

			Renderer::EndScene();
		}

		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(m_MainWindow);
	}

}