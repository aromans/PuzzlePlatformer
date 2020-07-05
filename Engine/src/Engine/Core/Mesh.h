#pragma once

#include "Engine/Core/Vertex.h"
#include "Engine/Core/Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {

	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		Mesh(const std::vector<AnimVertex>& vertices, const std::vector<unsigned int>& indices);
		~Mesh();

		std::vector<Vertex> Vertices() const { return m_Vertices; }
		std::vector<AnimVertex> AnimVertices() const { return m_AnimVertices; }
		std::vector<unsigned int> Indices() const { return m_Indices; }

		GLsizeiptr VerticesSize() const { return sizeof(Vertex) * m_Vertices.size(); }
		GLsizeiptr AnimVerticesSize() const { return sizeof(AnimVertex) * m_AnimVertices.size(); }
		GLsizeiptr IndicesSize() const { return sizeof(unsigned int) * m_Indices.size(); }

		GLsizei IndexCount() const { return m_Indices.size(); }
		GLsizei VertexCount() const { return m_Vertices.size(); }
		GLsizei AnimVertexCount() const { return m_AnimVertices.size(); }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<AnimVertex> m_AnimVertices;
		std::vector<unsigned int> m_Indices;
	};

}

