#include "Texture.h"

Texture::Texture(const char* path) : m_TextureID(0), m_Width(0), m_Height(0), m_BitDepth(0), m_FileLocation(path) { }

Texture::~Texture() { Clear(); }

bool Texture::LoadTexture(GLint param)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texData = stbi_load(m_FileLocation, &m_Width, &m_Height, &m_BitDepth, 0);

	if (!texData) {
		printf("Failed to find: %s\n", m_FileLocation);
		return false;
	}

	// Generate and Binding Texture 
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	// How it handles the texture when going around the edges for (s)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	// How it handles the texture when going around the edges for (t)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	// How it handles the texture when zooming out on the image 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// How it handles the texture when zooming in on the image 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);	// Automatically generates the MipMap

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);

	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Clear()
{
	glDeleteTextures(1, &m_TextureID);
	m_TextureID = 0;
	m_Width = 0;
	m_Height = 0;
	m_BitDepth = 0;
	m_FileLocation = "";
}
