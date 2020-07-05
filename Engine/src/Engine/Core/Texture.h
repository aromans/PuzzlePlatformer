#pragma once

#include "Engine/Vendor/stb_image.h"
#include <glad/glad.h>

namespace Engine {
	class Texture
	{
	public:
		Texture() {}
		Texture(const char* path);
		~Texture();

		bool LoadTexture(GLint param);
		void UseTexture(GLuint i);
		void Clear();

	private:
		GLuint m_TextureID;
		int m_Width, m_Height;
		int m_BitDepth;

		const char* m_FileLocation;
	};
}