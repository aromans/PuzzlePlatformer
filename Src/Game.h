#pragma once

#include "Constants.h"
#include ".\Renderer\Shader.h"
#include "Camera.h"
#include "Texture.h"
#include ".\Lighting\DirectionalLight.h"
#include ".\Lighting\PointLight.h"
#include ".\Lighting\SpotLight.h"
#include "Material.h"
#include "Vertex.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Model.h"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

namespace Engine {
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

		bool IsRunning() const { return !glfwWindowShouldClose(m_MainWindow); }

	private:
		// ** Temporary User Input Methods ** //
		void CreateCallBacks();

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

		// TODO: Level Class 
		int lvl_width;
		int lvl_height;

		bool m_IsRunning;

		glm::mat4 proj;
		glm::mat4 ortho;

		// Scene Lighting
		DirectionalLight m_Light;
		std::vector<PointLight> m_PointLights;
		std::vector<SpotLight> m_SpotLights;

		std::vector<Model*> m_Models;

		std::vector<Shader*> m_ShaderList;

		Shader m_DirectionalShadowShader;
	};
}