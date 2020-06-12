#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 position, GLfloat con, GLfloat lin, GLfloat exp,
			   glm::vec3 ambientColor, GLfloat ambientIntesnity, GLfloat diffuseIntensity);
	~PointLight();

	virtual void SendToShader(Shader& shader, const int& i);
	virtual void SendToShader(Shader& shader);

protected:
	glm::vec3 m_Position;

	GLfloat m_Constant;	// c
	GLfloat m_Linear;	// bx
	GLfloat m_Exponent;	// ax^2
};

