#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertex_code, const char* fragment_code);
	void CreateFromFiles(const char* vertex_path, const char* fragment_path);

	std::string ReadFile(const char* path);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void UseShader();
	void Clear();

private:
	void CompileShader(const char* vertex_code, const char* fragment_code);
	void AddShader(const GLuint& program, const char* shaderCode, GLenum shaderType);

private:
	GLuint m_ShaderID;
	GLuint m_UniformProjection;
	GLuint m_UniformModel;
};

