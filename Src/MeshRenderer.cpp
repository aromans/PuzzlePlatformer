#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : m_VAO(0), m_VBO(0), m_IBO(0), m_IndexCount(0) { }

MeshRenderer::~MeshRenderer() { Clear(); }

void MeshRenderer::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int num_of_vertices, unsigned int num_of_indices)
{
	m_IndexCount = num_of_indices;

	// Generate and Bind Vertex Array Object(s)
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Generate and Bind Index Buffer Object(s)
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * num_of_indices, indices, GL_STATIC_DRAW);

	// Generate and Bind Vertex Buffer Object(s)
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * num_of_vertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbinding Vertex Buffer Object(s)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbinding Vertex Array Object(s)
	glBindVertexArray(0);

	// Unbinding the Index Buffer Object(s)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshRenderer::Render()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshRenderer::Clear()
{
	if (m_IBO != 0) {
		glDeleteBuffers(1, &m_IBO);
		m_IBO = 0;
	}

	if (m_VBO != 0) {
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}

	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	m_IndexCount = 0;
}
