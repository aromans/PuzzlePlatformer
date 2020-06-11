#include "Material.h"

void Material::SendToShader(Shader& shader)
{
	shader.SetVec3f(m_Specular, "material.specular");
}
