#include "Shader.h"

Shader::Shader() : m_ShaderID(0), m_UniformModel(0), m_UniformProjection(0), m_UniformView(0) { }

Shader::~Shader() { Clear(); }

void Shader::CreateFromString(const char* vertex_code, const char* fragment_code)
{
	CompileShader(vertex_code, fragment_code);
}

void Shader::CreateFromFiles(const char* vertex_path, const char* fragment_path) 
{
	std::string vertex_shader = ReadFile(vertex_path);
	std::string fragment_shader = ReadFile(fragment_path);
	CompileShader(vertex_shader.c_str(), fragment_shader.c_str());
}

std::string Shader::ReadFile(const char* path) 
{
	std::string line{""};
	std::string content{""};
	std::ifstream filestream(path);

	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			content.append(line + "\n");
		}
	} else {
		printf("Error loading from the path '%s'!", path);
	}

	filestream.close();
	return content;
}

GLuint Shader::GetProjectionLocation()
{
	return m_UniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return m_UniformModel;
}

GLuint Shader::GetViewLocation()
{
	return m_UniformView;
}

void Shader::UseShader()
{
	// TODO: Print Warning if ShaderID is 0 -> No Shader being used!
	glUseProgram(m_ShaderID);
}

void Shader::Clear()
{
	if (m_ShaderID != 0) {
		glDeleteProgram(m_ShaderID);
		m_ShaderID = 0;
	}

	m_UniformModel = 0;
	m_UniformProjection = 0;
	m_UniformView = 0;
}

void Shader::CompileShader(const char* vertex_code, const char* fragment_code)
{
	m_ShaderID = glCreateProgram();

	if (!m_ShaderID) {
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(m_ShaderID, vertex_code, GL_VERTEX_SHADER);
	AddShader(m_ShaderID, fragment_code, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(m_ShaderID);
	glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_ShaderID, sizeof(eLog), nullptr, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(m_ShaderID);
	glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_ShaderID, sizeof(eLog), nullptr, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	// Uniform Introduction
	m_UniformModel = glGetUniformLocation(m_ShaderID, "model");
	m_UniformProjection = glGetUniformLocation(m_ShaderID, "projection");
	m_UniformView = glGetUniformLocation(m_ShaderID, "view");
}

void Shader::AddShader(const GLuint& program, const char* shaderCode, GLenum shaderType)
{
	GLuint created_shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint code_length[1];
	code_length[0] = strlen(shaderCode);

	glShaderSource(created_shader, 1, code, code_length);
	glCompileShader(created_shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(created_shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(created_shader, sizeof(eLog), nullptr, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, created_shader);
}
