#pragma once

#include "Shader.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
public:
	Light() { 
		m_Color = glm::vec3(1.0f, 1.0f, 1.0f); 
		m_AmbientIntensity = 1.0f; 
		m_DiffuseIntensity = 1.0f;
	}

	Light(glm::vec3 ambientColor, GLfloat ambientIntesnity, GLfloat diffuseIntensity) {
		m_Color = ambientColor;
		m_AmbientIntensity = ambientIntesnity;
		m_DiffuseIntensity = diffuseIntensity;
	}

	~Light() {}

	virtual void SendToShader(Shader& shader) = 0;

protected:
	glm::vec3 m_Color;
	GLfloat m_AmbientIntensity;
	GLfloat m_DiffuseIntensity;
};