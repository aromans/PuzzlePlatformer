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

	GLsizeiptr VerticesSize() const { return sizeof(Vertex) * m_Vertices.size(); }
	GLsizeiptr IndicesSize() const { return sizeof(unsigned int) * m_Indices.size(); }

	GLsizei IndexCount() const { return m_Indices.size(); }
	GLsizei VertexCount() const { return m_Vertices.size(); }

private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
};

