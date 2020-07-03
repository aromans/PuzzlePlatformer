#include "Epch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Engine {
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferElement& element) : m_Element(element)
	{
		m_FBO = 0;
		m_DepthId = 0;
		Initialize();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_DepthId);
		glDeleteTextures(1, &m_ColorId);
	}

	void OpenGLFrameBuffer::Initialize()
	{
		//if (m_FBO) {
		//	glDeleteFramebuffers(1, &m_FBO);
		//	glDeleteTextures(1, &m_DepthId);
		//	//glDeleteTextures(1, &m_ColorId);
		//}

		// Create Frame Buffer
		glGenFramebuffers(1, &m_FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		// Color Buffer
		glGenTextures(1, &m_ColorId);
		glBindTexture(GL_TEXTURE_2D, m_ColorId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Element.Width, m_Element.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		// How it handles the texture when zooming out on the image
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// How it handles the texture when zooming in on the image
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorId, 0);

		// Depth Buffer
		glGenRenderbuffers(1, &m_DepthId);
		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Element.Width, m_Element.Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthId);

		// Did FrameBuffer initialize correctly?
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			ENG_CORE_ERROR("Framebuffer is incomplete and did NOT initialize!");
		}

		// Unbind Frame Buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, m_Element.Width, m_Element.Height);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}