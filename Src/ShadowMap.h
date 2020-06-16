#pragma once

#include "Shader.h"

#include <stdio.h>

#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum texture_unit);
	virtual void Set(Shader* shader);

	GLuint GetShadowWidth() const { return m_ShadowWidth; }
	GLuint GetShadowHeight() const { return m_ShadowHeight; }

private:
	GLuint m_FBO;
	GLuint m_ShadowMapID;
	GLuint m_ShadowWidth, m_ShadowHeight;
};

