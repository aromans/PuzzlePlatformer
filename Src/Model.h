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
		Model(std::string obj_path, std::string albedo, std::string normal, Shader* shader);
		~Model();

		void Render(bool& pass);

	protected:
		std::unique_ptr<VAO> m_VertexArray;
		Material* m_Material;
		Mesh* m_Mesh;
	};

}

