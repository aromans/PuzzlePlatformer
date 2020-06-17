#pragma once

#include "VAO.h"

#include <memory>

namespace Engine {

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const VAO& vertexArray);
	};

}

