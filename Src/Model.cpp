#include "Model.h"

#include "Renderer/Renderer.h"
#include "ObjLoader.h"

#include <GL\glew.h>

namespace Engine {

	Model::Model(std::string obj_path, std::string albedo, std::string normal, Shader* shader)
	{
		auto parsed_obj = LoadOBJ(obj_path.c_str());
		m_Mesh = new Mesh(std::get<0>(parsed_obj), std::get<1>(parsed_obj));

		m_Material = new Material(shader, albedo, normal, "");
		m_Material->SetProperties(glm::vec3(.0f), .0f);

		m_Shader = shader;

		m_VertexArray.reset(VAO::Create());

		// Index Buffer Creation
		IBO* indexBuffer;
		indexBuffer = IBO::Create(&m_Mesh->Indices()[0], m_Mesh->IndicesSize());

		// Vertex Buffer Creation
		VBO* vertexBuffer;
		vertexBuffer = VBO::Create(&m_Mesh->Vertices()[0], m_Mesh->VerticesSize());

		std::vector<BufferElement> layout = {
			{ 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position) },
			{ 2, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord) },
			{ 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal) },
			{ 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, averagedTangent) },
			{ 3, sizeof(Vertex), (GLvoid*)offsetof(Vertex, averagedBitangent) }
		};

		vertexBuffer->AddAttributes(layout);

		m_VertexArray->AddIBO(*indexBuffer);
		m_VertexArray->AddVBO(*vertexBuffer);
	}

	Model::~Model()
	{
		m_VertexArray.release();
		delete m_Material;
	}

	void Model::Render(bool& pass)
	{
		m_Shader->Set1i(0, "IsAnimated");
		m_Material->AssignMaterial(pass);
		Renderer::Submit(*m_VertexArray);
	}

}
