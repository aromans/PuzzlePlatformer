#include "VAO.h"

#include "OpenGL/OpenGLVAO.h"

namespace Engine {

	VAO* VAO::Create() {
		return new OpenGLVAO();
	}

}