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

	Camera(glm::vec3 position, GLfloat pitch, GLfloat yaw);
	Camera(glm::vec3 position, glm::vec3 target);

	~Camera();

	void Move(bool* keys, double& dt);

	glm::vec3 GetPosition() const { return m_Position; }

	GLfloat DistanceFromTarget() const {
		return m_Distance;
	}

	glm::mat4 CalculateViewMatrix();

private:
	void CalculateLocalPosition();

private:
	// Camera Position & Look At target if exists
	glm::vec3 m_Position;
	glm::vec3 m_TargetPosition;
	glm::vec3 m_Direction;
	GLfloat m_Distance;
	GLfloat m_Theta;

	// Local Positions for Camera Orientation
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

	glm::vec3 m_WorldUp;
};

