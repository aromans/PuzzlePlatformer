#pragma once

#include "Constants.h"
#include "MeshRenderer.h"
#include "Shader.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

class Game {
public:
	Game() {}
	~Game();

	bool Initialize();	// The Game Engine's Awake
	void Start();		// The Game Engine's Start
	void HandleInput();
	void Update();		
	void Render();

	void CreateMesh();
	void CreateShader();

	bool IsRunning() const { return !glfwWindowShouldClose(mainWindow); }

private:
	GLFWwindow* mainWindow;
	GLint m_BufferWidth, m_BufferHeight;
	bool m_IsRunning;
	glm::mat4 proj;

	std::vector<MeshRenderer*> m_MeshList;
	std::vector<Shader*> m_ShaderList;
};