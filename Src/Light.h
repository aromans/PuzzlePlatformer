#pragma once

#include "Shader.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
public:
	Light() { 
		m_Color = glm::vec3(1.0f, 1.0f, 1.0f); 
		m_AmbientIntensity = 1.0f; 
		m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		m_DiffuseIntensity = 0.0f;
	}

	Light(glm::vec3 ambientColor, GLfloat ambientIntesnity, glm::vec3 direction, GLfloat diffuseIntensity) {
		m_Color = ambientColor;
		m_AmbientIntensity = ambientIntesnity;
		m_Direction = direction;
		m_DiffuseIntensity = diffuseIntensity;
	}

	~Light() {}

	void UseLight(Shader& shader);

private:
	// Ambient
	glm::vec3 m_Color;
	GLfloat m_AmbientIntensity;

	//Diffuse
	glm::vec3 m_Direction;
	GLfloat m_DiffuseIntensity;
};