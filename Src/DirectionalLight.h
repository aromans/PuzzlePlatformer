#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 direction, 
					 glm::vec3 ambientColor, 
					 GLfloat ambientIntesnity, 
					 GLfloat diffuseIntensity);
	~DirectionalLight();

	virtual void SendToShader(Shader& shader) override;

protected:
	glm::vec3 m_Direction;
};

