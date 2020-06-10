#pragma once

#include <GL\glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture() {}
	Texture(const char* path);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void Clear();

private:
	GLuint m_TextureID;
	int m_Width, m_Height;
	int m_BitDepth;

	const char* m_FileLocation;
};

