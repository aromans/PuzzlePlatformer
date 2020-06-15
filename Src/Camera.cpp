#include "Camera.h"

Camera::Camera() { }

Camera::Camera(glm::vec3 position, GLfloat pitch, GLfloat yaw)
{
	m_Position = position;

	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_Theta = 0.f;

	m_Distance = (GLfloat)(glm::length(m_Direction));

	CalculateLocalPosition();
}

Camera::Camera(glm::vec3 position, glm::vec3 target)
{
	m_Position = position;
	m_TargetPosition = target;

	m_Direction = m_TargetPosition - m_Position;

	m_Theta = 0.f;

	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_Distance = (GLfloat)(glm::length(m_Direction));

	CalculateLocalPosition();
}

Camera::~Camera()
{
}

void Camera::Move(bool* keys, double& dt)
{
	if (keys[GLFW_KEY_D]) {
		m_Theta += 1.f;
	}	
	
	if (keys[GLFW_KEY_A]) {
		m_Theta -= 1.f;
	}

	GLfloat xOffset = (GLfloat)(m_Distance * sin(glm::radians(m_Theta)) - m_Position.x);
	GLfloat zOffset = (GLfloat)(m_Distance * cos(glm::radians(m_Theta)) - m_Position.z);
	m_Position.x += xOffset;
	m_Position.z += zOffset;
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(m_Position, m_TargetPosition, m_WorldUp);
}

void Camera::CalculateLocalPosition()
{
	m_Front = glm::normalize(m_Direction);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
