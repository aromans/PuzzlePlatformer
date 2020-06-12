#pragma once

#include "Vertex.h"
#include "Mesh.h"

#include <vector>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>

class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	void CreateMesh(Mesh& mesh);
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices);
	void Render();
	void Clear();

private:
	GLuint m_VAO, m_VBO, m_IBO;
	GLsizei m_IndexCount;
	GLsizei m_VertexCount;
};

