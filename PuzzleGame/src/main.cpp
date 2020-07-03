#include <Engine.h>

#include "Camera.h"

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
		m_Camera = Camera(glm::vec3(0.0f, 2.f, 4.f), cameraTarget);

		/* Shader Initialization */
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
	Camera m_Camera;

	int lvl_height, lvl_width;
	std::vector<int> tree_locations;
};

Engine::Application* Engine::CreateApplication()
{
	return new Game();
}