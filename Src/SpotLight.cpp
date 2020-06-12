#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	m_Direction = glm::vec3(0.0f, -1.0f, 0.0f);
	m_Edge = 0.0f;
	m_ProcEdge = cosf(glm::radians(m_Edge));
}

SpotLight::SpotLight(glm::vec3 direction, GLfloat edge,
	glm::vec3 position, GLfloat con, GLfloat lin, GLfloat exp,
	glm::vec3 ambientColor, GLfloat ambientIntesnity, GLfloat diffuseIntensity) : PointLight(position, con, lin, exp, ambientColor, ambientIntesnity, diffuseIntensity)
{
	m_Direction = glm::normalize(direction);
	m_Edge = edge;
	m_ProcEdge = cosf(glm::radians(m_Edge));
}

SpotLight::~SpotLight() { }

void SpotLight::SetFlash(const glm::vec3& pos, const glm::vec3& dir)
{
	m_Position = pos;
	m_Direction = glm::normalize(dir);
}

void SpotLight::SendToShader(Shader& shader, const int& i)
{
	std::string prefix = "spotLights[" + std::to_string(i) + "].";

	// Spot Light Settings
	shader.SetVec3f(m_Direction, (prefix + "direction").c_str());
	shader.Set1f(m_ProcEdge, (prefix + "edge").c_str());

	// Point Light Settings
	shader.SetVec3f(m_Position, (prefix + "base.position").c_str());
	shader.Set1f(m_Constant, (prefix + "base.constant").c_str());
	shader.Set1f(m_Linear, (prefix + "base.linear").c_str());
	shader.Set1f(m_Exponent, (prefix + "base.exponent").c_str());

	// Ambient && Diffuse Settings 
	shader.SetVec3f(m_Color, (prefix + "base.base.color").c_str());
	shader.Set1f(m_AmbientIntensity, (prefix + "base.base.ambientIntensity").c_str());
	shader.Set1f(m_DiffuseIntensity, (prefix + "base.base.diffuseIntensity").c_str());
}

void SpotLight::SendToShader(Shader& shader) { }