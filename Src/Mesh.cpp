#include "Mesh.h"

Mesh::Mesh()
{
	m_VertexInformation = {};
	m_Vertices = {};
	m_Indices = {};
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	m_VertexInformation = vertices;
	m_Indices = indices;

	//AssembleVertexData();
}

Mesh::~Mesh() { }

void Mesh::AssignAttributes(const GLuint& index, const GLint& size, const void* ptr)
{
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), ptr);
	glEnableVertexAttribArray(index);
}

void Mesh::AssembleVertexData()
{
	//for (Vertex& v : m_VertexInformation)
	//{
	//	// Vertex Position
	//	m_Vertices.push_back(v.position.x);
	//	m_Vertices.push_back(v.position.y);
	//	m_Vertices.push_back(v.position.z);

	//	// UV Coordinates
	//	m_Vertices.push_back(v.texcoord.x);
	//	m_Vertices.push_back(v.texcoord.y);

	//	// Normals
	//	m_Vertices.push_back(v.normal.x);
	//	m_Vertices.push_back(v.normal.y);
	//	m_Vertices.push_back(v.normal.z);
	//}
}
