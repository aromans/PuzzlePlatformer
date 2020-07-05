#include "Epch.h"
#include "Mesh.h"

namespace Engine {

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

	Mesh::Mesh(const std::vector<AnimVertex>& vertices, const std::vector<unsigned int>& indices)
	{
		m_AnimVertices = vertices;
		m_Indices = indices;
	}

	Mesh::~Mesh() {
	}

}
