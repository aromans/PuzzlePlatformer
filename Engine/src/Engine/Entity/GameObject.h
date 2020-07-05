#pragma once

#include "Epch.h"
#include <Engine/Core/Transform.h>
#include <Engine/Entity/Component.h>

namespace Engine {
	class GameObject
	{
	public:
		GameObject(const std::string& name) 
			: m_Name(name), m_Transform(glm::vec3(0.0f)) {}
		virtual ~GameObject() = default;

		virtual void OnAwake() {} 
		virtual void OnUpdate(double& dt) {}
		virtual void OnRender() {}

		inline Transform& GetTransform() { return m_Transform; }

	public:
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			T* newComponent(new T(std::forward<Args>(args)...));
			m_Components.push_back(component);
			m_TypeMap[&typeid(*newComponent)] = newComponent;
			return *newComponent;
		}

		template <typename T>
		T* GetComponent() {
			return std::static_cast<T*>(m_TypeMap[&typeid(T)]);
		}

		template <typename T>
		bool HasComponent(T& component) {
			return m_TypeMap.count(&typeid(T));
		}

	protected:
		std::string m_Name;
		Transform m_Transform;

		std::vector<Component*> m_Components;
		std::map<std::type_info*, Component*> m_TypeMap;

		bool m_IsActive;
	};
}

