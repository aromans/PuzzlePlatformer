#pragma once

#include "Engine/Core/Mesh.h"
#include "Engine/Core/Material.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VAO.h"

namespace Engine {

	class Model
	{
	public:
		Model() {}
		Model(std::string obj_path, std::string albedo, std::string normal, Shader* shader);
		virtual ~Model();

		virtual void Render(bool& pass);

	private:
		std::unique_ptr<VAO> m_VertexArray;
		Material* m_Material;
		Mesh* m_Mesh;
		Shader* m_Shader;
	};

}

