#pragma once

#include "Epch.h"
#include "Event.h"

namespace Engine {
	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y)
			: m_XPos(x), m_YPos(y) { }

		inline float GetXPos() const { return m_XPos; }
		inline float GetYPos() const { return m_YPos; }

		std::string ToString() const override {
			std::stringstream s;
			s << "MousedMovedEvent: (" << m_XPos << ", " << m_YPos << ")";
			return s.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
	private:
		float m_XPos, m_YPos;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) { }

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream s;
			s << "MousedScrolledEvent: (" << m_XOffset << ", " << m_YOffset << ")";
			return s.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput);
	protected:
		MouseButtonEvent(int button) : m_Button(button) { }

		int m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override {
			std::stringstream s;
			s << "MouseButtonPressedEvent: " << m_Button;
			return s.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override {
			std::stringstream s;
			s << "MouseButtonReleasedEvent: " << m_Button;
			return s.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
}