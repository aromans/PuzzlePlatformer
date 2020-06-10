#pragma once

#include "Shader.h"

#include <glm\glm.hpp>

class Material
{
public:
	Material(glm::vec3 specular)
	{
		this->m_Specular = specular;
	}

	~Material() {}

	void SendToShader(Shader& program);

private:
	glm::vec3 m_Specular;
};

