#include "Epch.h"
#include "FrameBuffer.h"

#include "Engine/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Engine {
	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferElement& element) {
		return std::make_shared<OpenGLFrameBuffer>(element);
	}
}