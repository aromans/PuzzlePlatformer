#pragma once

#include <GL\glew.h>

class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices);
	void Render();
	void Clear();

private:
	GLuint m_VAO, m_VBO, m_IBO;
	GLsizei m_IndexCount;
};

