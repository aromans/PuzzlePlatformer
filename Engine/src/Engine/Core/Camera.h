#pragma once

#include "Epch.h"

namespace Engine {
	class Camera
	{
	public:
		virtual ~Camera() = default;

		virtual glm::mat4 CalculateViewMatrix() = 0;
		inline glm::vec3 GetPosition() const { return m_Position; }

	protected:
		// Camera Position & Look At target if exists
		glm::vec3 m_Position;
		glm::vec3 m_Direction;

		// Local Positions for Camera Orientation
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
	};
}