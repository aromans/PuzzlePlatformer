#include "GameCamera.h"

#include <Engine/Core/Input.h>

GameCamera::GameCamera()
{

}

GameCamera::GameCamera(glm::vec3 position, float pitch, float yaw)
{
	m_Position = position;

	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_Theta = 0.f;

	m_Distance = glm::length(m_Direction);

	CalculateLocalPosition();
}

GameCamera::GameCamera(glm::vec3 position, glm::vec3 target)
{
	m_Position = position;
	m_TargetPosition = target;

	m_Direction = m_TargetPosition - m_Position;

	m_Theta = 0.f;

	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_Distance = glm::length(m_Direction);

	CalculateLocalPosition();
}

GameCamera::~GameCamera()
{
}

void GameCamera::Move(double& dt)
{
	if (Engine::Input::IsKeyPressed(ENG_KEY_D) || Engine::Input::IsKeyPressed(ENG_KEY_RIGHT)) { 
		m_Theta += 1.f;
	}	
	
	if (Engine::Input::IsKeyPressed(ENG_KEY_A) || Engine::Input::IsKeyPressed(ENG_KEY_LEFT)) {
		m_Theta -= 1.f;
	}

	if (Engine::Input::IsKeyPressed(ENG_KEY_W) || Engine::Input::IsKeyPressed(ENG_KEY_UP)) {
		m_Distance -= .1f;
	}

	if (Engine::Input::IsKeyPressed(ENG_KEY_S) || Engine::Input::IsKeyPressed(ENG_KEY_DOWN)) {
		m_Distance += 0.1f;
	}

	float xOffset = (m_Distance * sin(glm::radians(m_Theta)) - m_Position.x);
	float zOffset = (m_Distance * cos(glm::radians(m_Theta)) - m_Position.z);
	m_Position.x += xOffset;
	m_Position.z += zOffset;
}

void GameCamera::OnMouseMove(double& dt)
{
	std::pair<float, float> t = Engine::Input::GetMousePosition();

	if (Engine::Input::IsMouseButtonPressed(ENG_MOUSE_BUTTON_LEFT))
	{
		float xPos = t.first - m_MouseLastPos.x;
		double angleChange = xPos * 15.0f * dt;
		m_Theta -= angleChange;
	}

	m_MouseLastPos = glm::vec2(t.first, t.second);
}

glm::mat4 GameCamera::CalculateViewMatrix()
{
	return glm::lookAt(m_Position, m_TargetPosition, m_WorldUp);
}

void GameCamera::CalculateLocalPosition()
{
	m_Front = glm::normalize(m_Direction);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
