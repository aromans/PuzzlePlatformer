#pragma once

#include <glm/glm.hpp>
#include <Engine/Core/Camera.h>

class GameCamera : public Engine::Camera
{
public:
	GameCamera();
	GameCamera(glm::vec3 position, float pitch, float yaw);
	GameCamera(glm::vec3 position, glm::vec3 target);

	virtual ~GameCamera() override;

	void Move(double& dt);
	void OnMouseMove(double& dt);

	virtual glm::mat4 CalculateViewMatrix() override;

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline float DistanceFromTarget() const { return m_Distance; }

private:
	void CalculateLocalPosition();

private:
	glm::vec3 m_TargetPosition;
	float m_Distance;
	float m_Theta;

	glm::vec2 m_MouseLastPos;
};

