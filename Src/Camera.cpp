#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat move_speed, GLfloat turn_speed)
	: m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch), m_MovementSpeed(move_speed), m_TurnSpeed(turn_speed)
{
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

	Update();
}

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Update()
{
	m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(m_Front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::Move(bool* keys, GLfloat deltaTime)
{
	if (keys[GLFW_KEY_W]) {
		m_Position += m_Front * m_MovementSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_S]) {
		m_Position -= m_Front * m_MovementSpeed * deltaTime;
	}	
	
	if (keys[GLFW_KEY_D]) {
		m_Position += m_Right * m_MovementSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_A]) {
		m_Position -= m_Right * m_MovementSpeed * deltaTime;
	}
}

// TODO: Mouse Sensitivity is REALLY high - should refactor and improve upon later . . .
void Camera::Rotate(std::pair<GLfloat, GLfloat> rotation_delta)
{
	GLfloat xChange = rotation_delta.first * m_TurnSpeed; 
	GLfloat yChange = rotation_delta.second * m_TurnSpeed;

	m_Yaw += xChange;
	m_Pitch += yChange;

	if (m_Pitch > 89.0f) {
		m_Pitch = 89.0f;
	} else if (m_Pitch < -89.0f) {
		m_Pitch = -89.0f;
	}

	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

