#include "PointLight.h"

PointLight::PointLight() : Light()
{
	m_Position = glm::vec3(0.0f);
	m_Constant = 1.0f;
	m_Linear = 0.0f;
	m_Exponent = 0.0f;
}

PointLight::PointLight(glm::vec3 position, GLfloat con, GLfloat lin, GLfloat exp,
	glm::vec3 ambientColor, GLfloat ambientIntesnity, GLfloat diffuseIntensity) : Light(ambientColor, ambientIntesnity, diffuseIntensity)
{
	m_Position = position;
	m_Constant = con;
	m_Linear = lin;
	m_Exponent = exp;
}

PointLight::~PointLight() { }

void PointLight::SendToShader(Shader& shader, const int& i)
{
	std::string prefix = "pointLights[" + std::to_string(i) + "].";

	shader.SetVec3f(m_Position, ("pointLightPositions[" + std::to_string(i) + "]").c_str());

	// Point Light Settings
	shader.SetVec3f(m_Position, (prefix + "position").c_str());
	shader.Set1f(m_Constant, (prefix + "constant").c_str());
	shader.Set1f(m_Linear, (prefix + "linear").c_str());
	shader.Set1f(m_Exponent, (prefix + "exponent").c_str());

	// Ambient && Diffuse Settings 
	shader.SetVec3f(this->m_Color, (prefix + "base.color").c_str());
	shader.Set1f(m_AmbientIntensity, (prefix + "base.ambientIntensity").c_str());
	shader.Set1f(m_DiffuseIntensity, (prefix + "base.diffuseIntensity").c_str());
}

void PointLight::SendToShader(Shader& shader) { }
