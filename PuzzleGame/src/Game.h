#pragma once

//#include <Engine.h>
//
//class Game : public Engine::Application
//{
//public:
//	Game();
//	~Game();
//};

//#include "Constants.h"
//#include "Window.h"
//#include "Events\Event.h"
//#include "Events\ApplicationEvent.h"
//#include "Events\MouseEvent.h"
//#include "Events\KeyEvent.h"
//#include ".\Renderer\Shader.h"
//#include "Camera.h"
//#include "Texture.h"
//#include ".\Lighting\DirectionalLight.h"
//#include ".\Lighting\PointLight.h"
//#include ".\Lighting\SpotLight.h"
//#include "Material.h"
//#include "Vertex.h"
//#include "ObjLoader.h"
//#include "Mesh.h"
//#include "Model.h"
//#include ".\Animation\AnimationModel.h"
//#include ".\Animation\Animator.h"
//
//#include "Character.h"
//
//#include <GLFW\glfw3.h>
//#include <glm\glm.hpp>
//#include <glm\gtc\matrix_transform.hpp>
//#include <glm\gtc\type_ptr.hpp>
//#include <vector>
//#include <memory>
//#include <functional>

//namespace Engine {
//
//	class Game {
//		// --- METHODS ---
//	public:
//		Game();
//		~Game();
//
//	//	bool Initialize();	// The Game Engine's Awake
//	//	void Start();		// The Game Engine's Start
//	//	void Update(double dt);
//	//	void Render();
//
//	//	void OnEvent(Event& e);
//	//	bool OnWindowClose(WindowCloseEvent& e);
//
//	//	bool IsRunning() const { return !glfwWindowShouldClose((GLFWwindow*)m_Window->GetNativeWindow()); }
//
//	//	inline Window& GetWindow() const { return *m_Window; }
//	//	inline static Game& Get() { return *s_Instance; }
//
//	//private:
//
//	//	// TODO: Refactor into appropriate class . . .
//	//	void DirectionalShadowMapPass(DirectionalLight* light, Shader* shader);
//	//	void RenderScene(Shader* shader, bool pass);
//	//	void RenderPass(Shader* shader);
//
//	// --- VARIABLES ---
//	private:
//
//		//GLFWwindow* m_MainWindow;
//		//std::unique_ptr<Window> m_Window;
//
//		//Camera m_MainCamera;
//		//GLint m_BufferWidth, m_BufferHeight;
//
//		//// TODO: Level Class
//		//int lvl_width;
//		//int lvl_height;
//
//		//bool m_IsRunning;
//
//		//glm::mat4 proj;
//		//glm::mat4 ortho;
//
//		//// Scene Lighting
//		//DirectionalLight m_Light;
//		//std::vector<PointLight> m_PointLights;
//		//std::vector<SpotLight> m_SpotLights;
//
//		//Character* toon;
//
//		//std::vector<Model*> m_Models;
//
//		//std::vector<Shader*> m_ShaderList;
//
//		//Shader m_DirectionalShadowShader;
//
//		static Game* s_Instance;
//	};
//}