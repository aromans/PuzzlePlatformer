#pragma once

#include <glad/glad.h>

namespace Engine {

	struct BufferElement {
		const void* Offset;
		int Count;
		int Size;
		GLenum Type;

		BufferElement(int count, int size, const void* offset) : Count(count), Size(size), Offset(offset), Type(GL_FLOAT) {}
		BufferElement(int count, int size, const void* offset, GLenum type) : Count(count), Size(size), Offset(offset), Type(type) {}
	};

	class VBO
	{
	public:
		virtual ~VBO() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddAttributes(const std::vector<BufferElement>& elements) = 0;
		virtual const std::vector<BufferElement>& GetBufferElements() const = 0;

		virtual size_t GetCount() const = 0;

		static VBO* Create(const void* data, size_t size);
	};

	class IBO
	{
	public:
		virtual ~IBO() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual size_t GetCount() const = 0;

		static IBO* Create(const void* data, size_t size);
	};

}

