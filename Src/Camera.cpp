#include "Camera.h"

#include "Input.h"

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

	m_Distance = (GLdouble)(glm::length(m_Direction));

	CalculateLocalPosition();
}

Camera::~Camera()
{
}

void Camera::Move(double& dt)
{
	if (Engine::Input::IsKeyPressed(GLFW_KEY_D) || Engine::Input::IsKeyPressed(GLFW_KEY_RIGHT)) { 
		m_Theta += 1.f;
	}	
	
	if (Engine::Input::IsKeyPressed(GLFW_KEY_A) || Engine::Input::IsKeyPressed(GLFW_KEY_LEFT)) {
		m_Theta -= 1.f;
	}

	if (Engine::Input::IsKeyPressed(GLFW_KEY_W) || Engine::Input::IsKeyPressed(GLFW_KEY_UP)) {
		m_Distance -= .1f;
	}

	if (Engine::Input::IsKeyPressed(GLFW_KEY_S) || Engine::Input::IsKeyPressed(GLFW_KEY_DOWN)) {
		m_Distance += 0.1f;
	}

	GLfloat xOffset = (GLfloat)(m_Distance * sin(glm::radians(m_Theta)) - m_Position.x);
	GLfloat zOffset = (GLfloat)(m_Distance * cos(glm::radians(m_Theta)) - m_Position.z);
	m_Position.x += xOffset;
	m_Position.z += zOffset;
}

void Camera::OnMouseMove(double& dt)
{
	std::pair<float, float> t = Engine::Input::GetMousePosition();

	if (Engine::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) 
	{
		float xPos = t.first - m_MouseLastPos.x;
		double angleChange = xPos * 15.0f * dt;
		m_Theta -= angleChange;
	}

	m_MouseLastPos = glm::vec2(t.first, t.second);
}

void Camera::OnMouseScroll(GLFWwindow* window, double xPos, double yPos)
{
	m_Distance -= yPos * 0.6;
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
