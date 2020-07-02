#pragma once

#include "Engine/Renderer/Buffers.h"

namespace Engine {

	class OpenGLVBO : public VBO
	{
	public:
		OpenGLVBO(const void* data, size_t size);
		virtual ~OpenGLVBO();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddAttributes(const std::vector<BufferElement>& elements) { m_Elements = elements; }
		const std::vector<BufferElement>& GetBufferElements() const override { return m_Elements; }

		virtual size_t GetCount() const { return m_Count; }
	private:
		unsigned int m_VBO;
		std::vector<BufferElement> m_Elements;
		size_t m_Count;
	};

	class OpenGLIBO : public IBO
	{
	public:
		OpenGLIBO(const void* data, size_t size);
		virtual ~OpenGLIBO();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual size_t GetCount() const { return m_Count; }
	private:
		unsigned int m_IBO;
		size_t m_Count;
	};

}


