#pragma once

#include "Engine/Renderer/FrameBuffer.h"

namespace Engine {
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferElement& element);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Initialize() override;

		virtual unsigned int GetID() const override { return m_FBO; }
		virtual unsigned int GetDepthId() const override { return 0; }
		virtual unsigned int GetColorId() const override { return 0; }

		virtual const FrameBufferElement& GetElement() const override { return m_Element; }
	private:
		unsigned int m_FBO;
		unsigned int m_DepthId;
		unsigned int m_ColorId;
		FrameBufferElement m_Element;
	};
}
