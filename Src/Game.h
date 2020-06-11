#pragma once

#include "Constants.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

class Game {
// --- METHODS ---
public:
	Game() { 
		for (size_t i = 0; i < 1024; ++i) {
			Keys[i] = 0;
		}
	}

	~Game();

	bool Initialize();	// The Game Engine's Awake
	void Start();		// The Game Engine's Start
	void HandleInput(double deltaTime);
	void Update();		
	void Render();

	void CalculateAverageNormals(unsigned int* indices, unsigned int indexCount, GLfloat* vertices,
		unsigned int vertexCount, unsigned int vLength, unsigned int normalOffset);

	MeshRenderer* CreateMesh();
	MeshRenderer* CreateFloor();
	void CreateShader();

	bool IsRunning() const { return !glfwWindowShouldClose(m_MainWindow); }

private:
	// ** Temporary User Input Methods ** //
	void CreateCallBacks();

	std::pair<GLfloat, GLfloat> GetChangedPos() {
		std::pair<GLfloat, GLfloat> changed = ChangedPos;
		ChangedPos = std::make_pair(0.0f, 0.0f);
		return changed;
	}

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
	// ** END TEMPORARY ** // 

// --- VARIABLES ---
public:
	// ** Temporary User Input Variables ** //
	bool Keys[1024];
	std::pair<GLfloat, GLfloat> LastPos = std::make_pair(0.0f, 0.0f);
	std::pair<GLfloat, GLfloat> ChangedPos = std::make_pair(0.0f, 0.0f);
	bool MouseFirstMoved = true;
	// ** END TEMPORARY ** // 

private:
	GLFWwindow* m_MainWindow;
	Camera m_MainCamera;
	GLint m_BufferWidth, m_BufferHeight;

	bool m_IsRunning;
	glm::mat4 proj;

	Texture m_BrickTexture;
	Texture m_DirtTexture;

	DirectionalLight m_Light;
	std::vector<PointLight> m_PointLights;

	Material m_MaterialOne;

	std::vector<MeshRenderer*> m_MeshList;
	std::vector<Shader*> m_ShaderList;
};