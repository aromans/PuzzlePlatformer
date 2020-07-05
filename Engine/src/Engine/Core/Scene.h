#pragma once

#include "Engine/Core/Camera.h"
#include "Engine/Lights/DirectionalLight.h"
#include "Engine/Lights/PointLight.h"
#include "Engine/Lights/SpotLight.h"

#include "Engine/Entity/GameObject.h"

namespace Engine {
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnAwake();
		void OnUpdate(double& dt);

		void AddSceneCamera(const Camera& camera);
		Camera& GetCamera() { return *m_SceneCamera; }

		void AddGameObject(GameObject* object);
		GameObject* CreateGameObject(const std::string& name);
		

	public:
		std::shared_ptr<Camera> m_SceneCamera;
		DirectionalLight m_SunLight;
		std::vector<PointLight> m_PointLights;
		std::vector<SpotLight> m_SpotLights;

		std::vector<GameObject> m_Entities;
	};
}