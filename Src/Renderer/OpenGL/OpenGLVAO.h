#pragma once

#include "../VAO.h"
#include "../Buffers.h"

namespace Engine {

	class OpenGLVAO : public VAO
	{
	public:
		OpenGLVAO();
		virtual ~OpenGLVAO();

		virtual void Bind() const override;

		virtual void Unbind() const override;

		virtual void AddVBO(VBO& vbo) override;

		virtual void AddIBO(IBO& ibo) override;

		virtual const std::vector<VBO*>& GetVBOs() const { return m_VertexBuffer; }

		virtual const IBO* GetIBO() const { return m_IndexBuffer; }

	private:
		unsigned int m_VAO;
		std::vector<VBO*> m_VertexBuffer;
		IBO* m_IndexBuffer;
	};

}

