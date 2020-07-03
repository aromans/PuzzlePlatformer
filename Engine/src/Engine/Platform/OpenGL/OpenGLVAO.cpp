#include "Epch.h"
#include "OpenGLVAO.h"

#include <glad/glad.h>

namespace Engine {
	OpenGLVAO::OpenGLVAO()
	{
		glCreateVertexArrays(1, &m_VAO);
	}

	OpenGLVAO::~OpenGLVAO()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void OpenGLVAO::Bind() const
	{
		glBindVertexArray(m_VAO);
	}

	void OpenGLVAO::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVAO::AddVBO(VBO& vbo)
	{
		glBindVertexArray(m_VAO);
		vbo.Bind();

		const auto& layout = vbo.GetBufferElements();
		for (size_t i = 0; i < layout.size(); ++i)
		{
			glEnableVertexAttribArray(i);

			if (layout[i].Type == GL_FLOAT)
				glVertexAttribPointer(i, layout[i].Count, GL_FLOAT, GL_FALSE, layout[i].Size, layout[i].Offset);
			else if (layout[i].Type == GL_INT)
				glVertexAttribIPointer(i, layout[i].Count, GL_INT, layout[i].Size, layout[i].Offset);
		}

		m_VertexBuffer.push_back(&vbo);
	}

	void OpenGLVAO::AddIBO(IBO& ibo)
	{
		glBindVertexArray(m_VAO);
		ibo.Bind();

		m_IndexBuffer = &ibo;
	}
}