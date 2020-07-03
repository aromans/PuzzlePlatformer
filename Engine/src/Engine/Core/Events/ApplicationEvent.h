#pragma once

#include "Epch.h"
#include "Event.h"

namespace Engine {
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) { }

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream s;
			s << "WindowResizedEvent: Width-> " << m_Width << " | Height-> " << m_Height;
			return s.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() { }

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class OnTickEvent : public Event {
	public:
		OnTickEvent() { }

		EVENT_CLASS_TYPE(OnTick);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class OnUpdateEvent : public Event {
	public:
		OnUpdateEvent() { }

		EVENT_CLASS_TYPE(OnUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class OnRenderEvent : public Event {
	public:
		OnRenderEvent() { }

		EVENT_CLASS_TYPE(OnRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}