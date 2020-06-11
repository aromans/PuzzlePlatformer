#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambientColor, 
								   GLfloat ambientIntesnity, GLfloat diffuseIntensity) : Light(ambientColor, ambientIntesnity, diffuseIntensity)
		
{
	m_Direction = direction;
}

DirectionalLight::DirectionalLight() : Light()
{
	m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::~DirectionalLight() { }

void DirectionalLight::SendToShader(Shader& shader)
{
	shader.SetVec3f(m_Direction, "directionalLight.direction");

	shader.SetVec3f(m_Color, "directionalLight.base.color");
	shader.Set1f(m_AmbientIntensity, "directionalLight.base.ambientIntensity");
	shader.Set1f(m_DiffuseIntensity, "directionalLight.base.diffuseIntensity");
}
