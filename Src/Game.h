#pragma once

#include "Constants.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Vertex.h"
#include "ObjLoader.h"
#include "Mesh.h"

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
	void HandleInput();
	void Update(double dt);		
	void Render();

	void CalculateAverageNormals(unsigned int* indices, unsigned int indexCount, GLfloat* vertices,
		unsigned int vertexCount, unsigned int vLength, unsigned int normalOffset);

	MeshRenderer* CreateMesh();
	MeshRenderer* CreateFloor();

	bool IsRunning() const { return !glfwWindowShouldClose(m_MainWindow); }

private:
	// ** Temporary User Input Methods ** //
	void CreateCallBacks();

	//std::pair<GLfloat, GLfloat> GetChangedPos() {
	//	std::pair<GLfloat, GLfloat> changed = ChangedPos;
	//	ChangedPos = std::make_pair(0.0f, 0.0f);
	//	return changed;
	//}

	// TODO: Refactor into appropriate class . . . 
	void DirectionalShadowMapPass(DirectionalLight* light, Shader* shader);
	void RenderScene(Shader* shader, bool pass);
	void RenderPass(Shader* shader);

	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
	static void HandleMouseScroll(GLFWwindow* window, double xPos, double yPos);
	static void HandleMouseButton(GLFWwindow* window, int button, int action, int mods);
	// ** END TEMPORARY ** // 

// --- VARIABLES ---
public:
	// ** Temporary User Input Variables ** //
	bool Keys[1024];
	glm::vec2 LastPos = glm::vec2(0, 0);
	glm::vec2 LastScrollAmnt = glm::vec2(0, 0);
	glm::vec2 MouseDelta = glm::vec2(0, 0);
	glm::vec2 MouseScrollDelta = glm::vec2(0, 0);
	bool MouseFirstMoved = true;
	// ** END TEMPORARY ** // 

private:
	GLFWwindow* m_MainWindow;
	Camera m_MainCamera;
	GLint m_BufferWidth, m_BufferHeight;

	bool m_IsRunning;

	glm::mat4 proj;
	glm::mat4 ortho;

	// Scene Lighting
	DirectionalLight m_Light;
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;

	Mesh* m_DirtCube;
	Mesh* m_Tree;
	Mesh* m_Robo;
	Mesh* m_PineTree_Leaves;
	Mesh* m_PineTree_Bark;

	// Not Needed
	Material* m_DirtMat;
	Material* m_BrickMat;
	Material* m_DirtCubeMat;
	Material* m_TreeMat;
	Material* m_RoboMat;
	Material* m_PineTreeLeavesMat;
	Material* m_PineTreeBarkMat;

	// Scene Meshses && Shaders
	std::vector<MeshRenderer*> m_MeshList;
	std::vector<Shader*> m_ShaderList;

	Shader m_DirectionalShadowShader;
};