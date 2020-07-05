#include "Epch.h"
#include "Scene.h"

namespace Engine {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::AddSceneCamera(const Camera& camera)
	{
		m_SceneCamera = std::make_shared<Camera>(camera);
	}
}