#pragma once

#include "Shader.h"

#include <glm\glm.hpp>

class Material
{
public:
	Material() {}

	Material(glm::vec3 specular)
	{
		this->m_Specular = specular;
	}

	~Material() {}

	void SendToShader(Shader& shader);

private:
	glm::vec3 m_Specular;
};

