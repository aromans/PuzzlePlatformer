#include "Material.h"

//void Material::SendToShader(Shader& shader)
//{
//	shader.SetVec3f(m_Ambient, "material.ambient");
//	shader.SetVec3f(m_Diffuse, "material.diffuse");
//	shader.SetVec3f(m_Specular, "material.specular");
//}

Material::Material(Shader* shader, std::string diffuse_tex, std::string normal_tex)
{
	if (shader != nullptr) {
		m_Shader = shader;
	}
	else {
		printf("ERROR! Did not pass in either a vertex or fragment shader file, this is necessary to apply a material!");
		return;
	}

	if (diffuse_tex != "") {
		m_Diffuse = Texture(diffuse_tex.c_str());
		m_Diffuse.LoadTexture(GL_REPEAT);
	}

	m_NormalTex = normal_tex;
	if (normal_tex != "") {
		m_Normal = Texture(normal_tex.c_str());
		m_Normal.LoadTexture(GL_REPEAT);
	}
}

void Material::SetProperties(glm::vec3 specular, GLfloat shininess)
{
	m_Specular = specular;
	m_Shininess = shininess;
}

void Material::AssignMaterial(bool pass)
{
	if (pass == false) {
		ApplyUniforms();
		m_Shader->Use();

		m_Diffuse.UseTexture(0);
		m_Normal.UseTexture(1);	
	}
}

void Material::ApplyUniforms()
{
	m_Shader->Set1i(0, "material.diffuse");
	m_Shader->Set1i(1, "material.normal");
	m_Shader->SetVec3f(m_Specular, "material.specular");
	m_Shader->Set1f(m_Shininess, "material.shininess");
	m_Shader->Set1i(m_NormalTex != "", "material.has_normal_map");

}
