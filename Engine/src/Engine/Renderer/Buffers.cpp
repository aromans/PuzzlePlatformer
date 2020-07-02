#include "Epch.h"
#include "Buffers.h"

#include "Engine/Platform/OpenGL/OpenGLBuffers.h"

namespace Engine {

	VBO* VBO::Create(const void* data, size_t size) 
	{
		return new OpenGLVBO(data, size);
	}

	IBO* IBO::Create(const void* data, size_t size)
	{
		return new OpenGLIBO(data, size);
	}

}
