#include "OpenGLRenderer.h"

#include "../VAO.h"


namespace Engine {

	OpenGLRenderer::OpenGLRenderer(GLFWwindow* window) : m_WindowHandle(window)
	{

	}

	void OpenGLRenderer::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
	}

	void OpenGLRenderer::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void OpenGLRenderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
