#pragma once

#include "Constants.h"

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat move_speed, GLfloat turn_speed);
	~Camera();

	void Move(bool* keys, GLfloat deltaTime);
	void Rotate(std::pair<GLfloat, GLfloat> rotation_delta);
	glm::mat4 CalculateViewMatrix();

	glm::vec3 GetPosition() const { return m_Position; }

private:
	void Update();

private:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	GLfloat m_Yaw;
	GLfloat m_Pitch;

	GLfloat m_MovementSpeed;
	GLfloat m_TurnSpeed;
};

