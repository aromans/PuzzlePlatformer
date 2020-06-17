#pragma once

#include "Texture.h"
#include "Renderer/Shader.h"

#include <glm\glm.hpp>
#include <string>

class Material
{
public:
	Material() {}
	Material(Shader* shader, std::string diffuse_tex, std::string normal_tex);

	~Material() {}

	void SetProperties(glm::vec3 specular, GLfloat shininess);
	void AssignMaterial(bool pass);

private:

	void ApplyUniforms();

private:
	Shader* m_Shader;

	Texture m_Diffuse;
	Texture m_Normal;

	std::string m_NormalTex;

	glm::vec3 m_Specular;
	GLfloat m_Shininess;
};

