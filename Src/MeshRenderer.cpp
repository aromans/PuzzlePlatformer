#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() : m_VAO(0), m_VBO(0), m_IBO(0), m_IndexCount(0) { }

MeshRenderer::~MeshRenderer() { Clear(); }

void MeshRenderer::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int num_of_vertices, unsigned int num_of_indices, Material* material)
{
	m_Material = material;
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	// Unbinding Vertex Buffer Object(s)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbinding Vertex Array Object(s)
	glBindVertexArray(0);

	// Unbinding the Index Buffer Object(s)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshRenderer::CreateMesh(Mesh* mesh, Material* material)
{
	m_Mesh = mesh;
	m_Material = material;
	m_IndexCount = mesh->IndexCount();
	m_VertexCount = mesh->VertexCount();

	// Generate and Bind VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Generate and Bind IBO
	if (m_IndexCount > 0) {
		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->IndicesSize(), &mesh->Indices()[0], GL_STATIC_DRAW);
	}

	// Generate and Bind 
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->VerticesSize(), &mesh->Vertices()[0], GL_STATIC_DRAW);

	// Vertex Attributes
	mesh->AssignAttributes(0, 3, (GLvoid*)offsetof(Vertex, position));
	// UV Coordinate Attributes
	mesh->AssignAttributes(1, 2, (GLvoid*)offsetof(Vertex, texcoord));
	// Normal Attributes
	mesh->AssignAttributes(2, 3, (GLvoid*)offsetof(Vertex, normal));
	// Tangent Attributes
	mesh->AssignAttributes(3, 3, (GLvoid*)offsetof(Vertex, averagedTangent));
	// Bitangent Attributes
	mesh->AssignAttributes(4, 3, (GLvoid*)offsetof(Vertex, averagedBitangent));

	// Unbinding Vertex Buffer Object(s)
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbinding Vertex Array Object(s)
	glBindVertexArray(0);

	// Unbinding the Index Buffer Object(s)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshRenderer::Render(bool pass)
{
	m_Material->AssignMaterial(pass);

	glBindVertexArray(m_VAO);

	if (this->m_IndexCount != 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, (void*)0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, this->m_VertexCount);
	}

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
