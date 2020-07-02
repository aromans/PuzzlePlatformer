#include "Epch.h"
#include "VAO.h"

#include "Engine/Platform/OpenGL/OpenGLVAO.h"

namespace Engine {

	VAO* VAO::Create() {
		return new OpenGLVAO();
	}

}