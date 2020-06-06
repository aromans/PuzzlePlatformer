#pragma once

#include "Constants.h"
#include <GLFW\glfw3.h>

class Game {
public:
	bool Initialize();	// The Game Engine's Awake
	void Start();		// The Game Engine's Start
	void HandleInput();
	void Update();		
	void Render();

	void CreateTriangle();
	void AddShader(const GLuint& program, const char* shaderCode, GLenum shaderType);
	void CompileShaders();

	bool IsRunning() const { return !glfwWindowShouldClose(mainWindow); }

private:
	GLFWwindow* mainWindow;
	bool m_IsRunning;
};