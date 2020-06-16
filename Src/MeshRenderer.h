#pragma once

#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"

#include <vector>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>

class MeshRenderer
{
public:
	MeshRenderer();
	~MeshRenderer();

	void CreateMesh(Mesh* mesh, Material* material);
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices, Material* material);
	void Render(bool pass);
	void Clear();

	Mesh* GetMesh() const { return m_Mesh; }

private:
	Mesh* m_Mesh;
	Material* m_Material;

	GLuint m_VAO, m_VBO, m_IBO;
	GLuint m_TangentBuffer, m_BitangentBuffer;
	GLsizei m_IndexCount;
	GLsizei m_VertexCount;
};

