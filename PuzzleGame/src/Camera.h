#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, float pitch, float yaw);
	Camera(glm::vec3 position, glm::vec3 target);

	~Camera();

	void Move(double& dt);
	void OnMouseMove(double& dt);

	glm::mat4 CalculateViewMatrix();

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline float DistanceFromTarget() const { return m_Distance; }

private:
	void CalculateLocalPosition();

private:
	// Camera Position & Look At target if exists
	glm::vec3 m_Position;
	glm::vec3 m_TargetPosition;
	glm::vec3 m_Direction;
	float m_Distance;
	float m_Theta;

	// Local Positions for Camera Orientation
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;

	glm::vec3 m_WorldUp;

	glm::vec2 m_MouseLastPos;
};

