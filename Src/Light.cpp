#include "Light.h"

void Light::UseLight(Shader& shader)
{
	shader.SetVec3f(m_Color, "directionalLight.color");
	shader.Set1f(m_AmbientIntensity, "directionalLight.ambientIntensity");
	shader.SetVec3f(m_Direction, "directionalLight.direction");
	shader.Set1f(m_DiffuseIntensity, "directionalLight.diffuseIntensity");
}