#include "Mesh.h"

Mesh::Mesh()
{
	m_Vertices = {};
	m_Indices = {};
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	m_Vertices = vertices;
	m_Indices = indices;
	//m_Tangents = tangents;
	//m_Bitangents = bitangents;
}

Mesh::~Mesh() { 
}

void Mesh::AssignAttributes(const GLuint& index, const GLint& size, const void* ptr)
{
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr);
	glEnableVertexAttribArray(index);
}

void Mesh::AssignAttributes(const GLuint& index, const GLint& size, GLsizei stride, const void* ptr)
{
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, ptr);
	glEnableVertexAttribArray(index);
}
