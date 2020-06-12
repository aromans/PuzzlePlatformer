#pragma once

#include "Vertex.h"
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

class Mesh
{
public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	const Vertex* Vertices() const { return m_VertexInformation.data(); }
	const unsigned int* Indices() const { return m_Indices.data(); }

	GLsizeiptr VerticesSize() const { return sizeof(Vertex) * m_VertexInformation.size(); }
	GLsizeiptr IndicesSize() const { return sizeof(unsigned int) * m_Indices.size(); }

	GLsizei VertexAmount() const { return m_VertexInformation.size(); }

	//GLsizei VertexCount() const { return m_VertexInformation.size() * 3; }
	//GLsizei IndexCount() const { return m_Indices.size(); }

	void AssignAttributes(const GLuint& index, const GLint& size, const void* ptr);

private:
	void AssembleVertexData();

private:
	std::vector<Vertex> m_VertexInformation;

	std::vector<GLfloat> m_Vertices;
	std::vector<unsigned int> m_Indices;
};

