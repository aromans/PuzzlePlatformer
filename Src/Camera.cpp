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
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		m_Theta += 1.f;
	}	
	
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		m_Theta -= 1.f;
	}

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		m_Distance -= .1f;
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		m_Distance += 0.1f;
	}

	GLfloat xOffset = (GLfloat)(m_Distance * sin(glm::radians(m_Theta)) - m_Position.x);
	GLfloat zOffset = (GLfloat)(m_Distance * cos(glm::radians(m_Theta)) - m_Position.z);
	m_Position.x += xOffset;
	m_Position.z += zOffset;
}

float lastX = 0.0f;

void Camera::OnMouseMove(bool* keys, glm::vec2 currPos, glm::vec2 lastPos, double& dt)
{
	if (keys[GLFW_MOUSE_BUTTON_LEFT] && currPos.x != lastX && ((currPos.x - lastPos.x) > 1.f || (currPos.x - lastPos.x) < -1.f)) {
		float angleChange = currPos.x * 15.f * dt;
		m_Theta -= angleChange;
	}

	lastX = currPos.x;
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
