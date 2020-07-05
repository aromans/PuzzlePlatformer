#include <Engine.h>

#include "GameCamera.h"

class Game : public Engine::Application
{
public:
	Game() {}
	~Game() {}

	void Awake() override
	{
		/* TODO: Put this in a level/environment class */
		lvl_height = 3; lvl_width = 3;
		tree_locations = { 8 };// 4, 6, 7, 8, 12, 16, 17 };
		float x = lvl_width % 2 == 0 ? -0.5f : 0.0f;
		float z = -(lvl_height / 2) - (0.1f * (lvl_height / 2));
		z = lvl_height % 2 == 0 ? z + 0.5f : z;

		glm::vec3 cameraTarget = glm::vec3(x, -1.f, z);

		/* Camera Initialization */
		m_Camera = GameCamera(glm::vec3(0.0f, 2.f, 4.f), cameraTarget);
		m_ActiveScene->AddSceneCamera(m_Camera);

		/* Shader Initialization */
		Engine::Shader* normalShader = new Engine::Shader("Shaders/normalVShader.glsl", "Shaders/normalFShader.glsl");

		/* Light Initialization */
		Engine::DirectionalLight m_Light = Engine::DirectionalLight(glm::vec3(2.0f, 1.f, 2.f), 2048, 2048, glm::vec3(1.0f, 1.0f, 1.0f), .3f, 1.0f);
		m_ActiveScene->m_SunLight = m_Light;

		// Point Lights
		Engine::PointLight p1 = Engine::PointLight(glm::vec3(-1.1f, -2.f, 0.0f), 0.3f, 0.2f, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, .6f);
		m_ActiveScene->m_PointLights.push_back(p1);
		Engine::PointLight p2 = Engine::PointLight(glm::vec3(1.1f, -2.f, 0.0f), 0.3f, 0.2f, 0.1f, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 2.5f);
		m_ActiveScene->m_PointLights.push_back(p2);
		Engine::PointLight p3 = Engine::PointLight(glm::vec3(-1.1f, -2.f, -2.2f), 0.3f, 0.2f, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.2f);
		m_ActiveScene->m_PointLights.push_back(p3);

		Engine::SpotLight s1 = Engine::SpotLight(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f, glm::vec3(1.1f, -2.f, 0.0f), 1.0f, 0.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f);
		m_ActiveScene->m_SpotLights.push_back(s1);
		Engine::SpotLight s2 = Engine::SpotLight(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f, glm::vec3(-1.1f, -2.0f, -2.2f), 1.0f, 0.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.5f);
		m_ActiveScene->m_SpotLights.push_back(s2);

		/* GameObject Initialization */
		Engine::Model* dirtCube = new Engine::Model("ObjFiles/DirtCube.obj", "Textures/cube.png", "Textures/cube_normal.png", normalShader);
	}

	void OnShutdown() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void OnUpdate(double& dt) override
	{
		m_Camera.Move(dt);
		m_Camera.OnMouseMove(dt);
	}

private:
	GameCamera m_Camera;

	int lvl_height, lvl_width;
	std::vector<int> tree_locations;
};

Engine::Application* Engine::CreateApplication()
{
	return new Game();
}