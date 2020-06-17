#include "OpenGLBuffers.h"

#include <vector>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>

namespace Engine {

	OpenGLVBO::OpenGLVBO(const void* data, size_t size)
	{
		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLVBO::~OpenGLVBO()
	{
		glDeleteBuffers(1, &m_VBO);
	}

	void OpenGLVBO::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}

	void OpenGLVBO::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGLIBO::OpenGLIBO(const void* data, size_t size) : m_Count(size)
	{
		glCreateBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	OpenGLIBO::~OpenGLIBO()
	{
		glDeleteBuffers(1, &m_IBO);
	}

	void OpenGLIBO::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	}

	void OpenGLIBO::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
