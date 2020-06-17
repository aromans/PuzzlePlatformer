#include "DirectionalLight.h"

namespace Engine {

	DirectionalLight::DirectionalLight(glm::vec3 direction,
		GLuint shadowWidth, GLuint shadowHeight,
		glm::vec3 ambientColor,
		GLfloat ambientIntesnity, GLfloat diffuseIntensity)
		: Light(shadowWidth, shadowHeight, ambientColor, ambientIntesnity, diffuseIntensity)

	{
		m_Direction = direction;
		m_LightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
	}

	DirectionalLight::DirectionalLight() : Light()
	{
		m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
		m_LightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
	}

	DirectionalLight::~DirectionalLight() { }

	glm::mat4 DirectionalLight::CalculateLightTransform()
	{
		return m_LightProj * glm::lookAt(m_Direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void DirectionalLight::SendToShader(Shader& shader)
	{
		shader.SetVec3f(m_Direction, "directionalLight.direction");

		shader.SetVec3f(m_Color, "directionalLight.base.color");
		shader.Set1f(m_AmbientIntensity, "directionalLight.base.ambientIntensity");
		shader.Set1f(m_DiffuseIntensity, "directionalLight.base.diffuseIntensity");
	}

}
