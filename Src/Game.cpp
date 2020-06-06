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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	return true;
}

void Game::Start()
{
}

void Game::HandleInput()
{
	// Get + Handle user input events
	glfwPollEvents();
}

void Game::Update()
{
}

void Game::Render()
{
	// Clear window
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(mainWindow);
}
