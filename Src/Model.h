#pragma once

#include "Mesh.h"
#include ".\Renderer\Shader.h"
#include "Material.h"
#include ".\Renderer\VAO.h"

#include <memory>
#include <string.h>
#include <vector>

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

