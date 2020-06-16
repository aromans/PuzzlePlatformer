#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	m_FBO = 0;
	m_ShadowMapID = 0;
}

ShadowMap::~ShadowMap()
{
	if (m_FBO) {
		glDeleteFramebuffers(1, &m_FBO);
	}

	if (m_ShadowHeight) {
		glDeleteTextures(1, &m_ShadowMapID);
	}
}

bool ShadowMap::Init(GLuint width, GLuint height)
{
	m_ShadowWidth = width;
	m_ShadowHeight = height;

	glGenFramebuffers(1, &m_FBO);

	glGenTextures(1, &m_ShadowMapID);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMapID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ShadowWidth, m_ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// How it handles the texture when going around the edges for (s)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	// How it handles the texture when going around the edges for (t)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float bColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);
	// How it handles the texture when zooming out on the image 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// How it handles the texture when zooming in on the image 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMapID, 0);

	// Explicitly state we don't want color
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FRAMEBUFFER ERROR! Error: %i\n", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void ShadowMap::Read(GLenum texture_unit)
{
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMapID);
}

void ShadowMap::Set(Shader* shader)
{
	shader->Set1i(m_ShadowMapID, "directionalShadowMap");
}
