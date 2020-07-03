#pragma once

#include "Engine/Platform/OpenGL/OpenGLBuffers.h"

namespace Engine {
	class VAO
	{
	public:
		virtual ~VAO() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVBO(VBO& vbo) = 0;
		virtual void AddIBO(IBO& ibo) = 0;

		virtual const std::vector<VBO*>& GetVBOs() const = 0;
		virtual const IBO* GetIBO() const = 0;

		static VAO* Create();
	};
}
