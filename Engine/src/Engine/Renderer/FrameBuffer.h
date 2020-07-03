#pragma once

#include <memory>

namespace Engine {
	struct FrameBufferElement {
		uint32_t Width;
		uint32_t Height;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Initialize() = 0;

		virtual unsigned int GetID() const = 0;
		virtual unsigned int GetDepthId() const = 0;
		virtual unsigned int GetColorId() const = 0;

		virtual const FrameBufferElement& GetElement() const = 0;

		static std::shared_ptr<FrameBuffer> Create(const FrameBufferElement& element);
	};
}
