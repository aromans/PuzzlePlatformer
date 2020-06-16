#pragma once

#include "Shader.h"
#include "ShadowMap.h"

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Light {
public:
	Light() 
	{ 
		m_Color = glm::vec3(1.0f, 1.0f, 1.0f); 
		m_AmbientIntensity = 1.0f; 
		m_DiffuseIntensity = 1.0f;
	}

	Light(GLfloat shadowWidth, GLfloat shadowHeight,
		  glm::vec3 ambientColor, GLfloat ambientIntesnity, GLfloat diffuseIntensity) 
	{
		m_ShadowMap = new ShadowMap();
		m_ShadowMap->Init(shadowWidth, shadowHeight);

		m_Color = ambientColor;
		m_AmbientIntensity = ambientIntesnity;
		m_DiffuseIntensity = diffuseIntensity;
	}

	~Light() {}

	ShadowMap* GetShadowMap() const { return m_ShadowMap; }

	virtual void SendToShader(Shader& shader) = 0;

protected:
	glm::vec3 m_Color;
	GLfloat m_AmbientIntensity;
	GLfloat m_DiffuseIntensity;

	glm::mat4 m_LightProj;

	ShadowMap* m_ShadowMap;
};