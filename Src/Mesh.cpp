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
}

Mesh::~Mesh() { 
}
