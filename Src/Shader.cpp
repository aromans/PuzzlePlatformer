#include "Shader.h"

#pragma region Public Methods

void Shader::Use() { glUseProgram(this->m_ShaderID); }

void Shader::Unuse() { glUseProgram(0); }

void Shader::Set1i(GLint value, const GLchar* name)
{
	this->Use();

	glUniform1i(glGetUniformLocation(this->m_ShaderID, name), value);

	this->Unuse();
}

void Shader::Set1f(GLfloat value, const GLchar* name)
{
	this->Use();

	glUniform1f(glGetUniformLocation(this->m_ShaderID, name), value);

	this->Unuse();
}

void Shader::SetVec2f(glm::vec2 value, const GLchar* name)
{
	this->Use();

	glUniform2fv(glGetUniformLocation(this->m_ShaderID, name), 1, glm::value_ptr(value));

	this->Unuse();
}

void Shader::SetVec3f(glm::vec2 value, const GLchar* name)
{
	this->Use();

	glUniform3fv(glGetUniformLocation(this->m_ShaderID, name), 1, glm::value_ptr(value));

	this->Unuse();
}

void Shader::SetVec4f(glm::vec2 value, const GLchar* name)
{

	this->Use();

	glUniform4fv(glGetUniformLocation(this->m_ShaderID, name), 1, glm::value_ptr(value));

	this->Unuse();
}

void Shader::SetMat3f(glm::mat3 value, const GLchar* name)
{
	this->Use();

	glUniformMatrix3fv(glGetUniformLocation(this->m_ShaderID, name), 1, GL_FALSE, glm::value_ptr(value));

	this->Unuse();
}

void Shader::SetMat4f(glm::mat4 value, const GLchar* name)
{
	this->Use();

	glUniformMatrix4fv(glGetUniformLocation(this->m_ShaderID, name), 1, GL_FALSE, glm::value_ptr(value));

	this->Unuse();
}

#pragma endregion

#pragma region Private Methods

std::string Shader::ReadFile(const char* path)
{
	std::string line{ "" };
	std::string content{ "" };
	std::ifstream filestream(path);

	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			content.append(line + "\n");
		}
	}
	else {
		printf("ERROR: Error loading from the path '%s'!", path);
	}

	filestream.close();
	return content;
}

void Shader::CompileShader(GLuint vertexid, GLuint fragmentId, GLuint geometryId)
{
	m_ShaderID = glCreateProgram();

	if (!m_ShaderID) {
		printf("ERROR: Error creating shader program!\n");
		return;
	}

	glAttachShader(this->m_ShaderID, vertexid);

	if (geometryId)
		glAttachShader(this->m_ShaderID, geometryId);

	glAttachShader(this->m_ShaderID, fragmentId);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(m_ShaderID);
	glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_ShaderID, sizeof(eLog), nullptr, eLog);
		printf("ERROR: Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(m_ShaderID);
	glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_ShaderID, sizeof(eLog), nullptr, eLog);
		printf("ERROR: Error validating program: '%s'\n", eLog);
		return;
	}

	glUseProgram(0);
}

GLuint Shader::AddShader(const GLchar* shaderCode, GLenum shaderType)
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("ERROR: Error compiling the %d shader: '%s'\n", shaderType, eLog);
	}

	return shader;
}

#pragma endregion