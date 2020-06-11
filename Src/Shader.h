#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = "")
	{
		GLuint vertexId = AddShader(ReadFile(vertexFile).c_str(), GL_VERTEX_SHADER);

		GLuint geometryId = 0;
		if (geometryFile != "")
			geometryId = AddShader(ReadFile(geometryFile).c_str(), GL_GEOMETRY_SHADER);

		GLuint fragmentId = AddShader(ReadFile(fragmentFile).c_str(), GL_FRAGMENT_SHADER);

		this->CompileShader(vertexId, fragmentId, geometryId);

		glDeleteShader(vertexId);
		glDeleteShader(geometryId);
		glDeleteShader(fragmentId);
	}

	~Shader() { glDeleteProgram(this->m_ShaderID); }

	void Use();
	void Unuse();

	void Set1i(GLint value, const GLchar* name);
	void Set1f(GLfloat value, const GLchar* name);
	void SetVec2f(glm::vec2 value, const GLchar* name);
	void SetVec3f(glm::vec3 value, const GLchar* name);
	void SetVec4f(glm::vec4 value, const GLchar* name);
	void SetMat3f(glm::mat3 value, const GLchar* name);
	void SetMat4f(glm::mat4 value, const GLchar* name);

private:
	std::string ReadFile(const char* path);
	void CompileShader(GLuint vertexid, GLuint fragmentId, GLuint geometryId);
	GLuint AddShader(const GLchar* shaderCode, GLenum shaderType);

private:
	GLuint m_ShaderID;
};

