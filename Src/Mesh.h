#pragma once

#include "Vertex.h"
#include "Texture.h"

#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <string>

class Mesh
{
public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	std::vector<Vertex> Vertices() const { return m_Vertices; }
	std::vector<unsigned int> Indices() const { return m_Indices; }
	//std::vector<glm::vec3> Tangents() const { return m_Tangents; }
	//std::vector<glm::vec3> Bitangents() const { return m_Bitangents; }


	GLsizeiptr VerticesSize() const { return sizeof(Vertex) * m_Vertices.size(); }
	GLsizeiptr IndicesSize() const { return sizeof(unsigned int) * m_Indices.size(); }
	//GLsizeiptr TangentsSize() const { return sizeof(glm::vec3) * m_Tangents.size(); }
	//GLsizeiptr BitangentsSize() const { return sizeof(glm::vec3) * m_Bitangents.size(); }

	GLsizei IndexCount() const { return m_Indices.size(); }
	GLsizei VertexCount() const { return m_Vertices.size(); }
	//GLsizei TangentCount() const { return m_Tangents.size(); }
	//GLsizei BitangentCount() const { return m_Bitangents.size(); }

	void AssignAttributes(const GLuint& index, const GLint& size, const void* ptr);
	void AssignAttributes(const GLuint& index, const GLint& size, GLsizei stride, const void* ptr);

private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	//std::vector<glm::vec3> m_Tangents;
	//std::vector<glm::vec3> m_Bitangents;
};

