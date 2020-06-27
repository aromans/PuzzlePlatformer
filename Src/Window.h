#pragma once

#include "Events/Event.h"

#include <functional>

namespace Engine {

	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Puzzle Platformer",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) { }
	};

	class Window {
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() { }

		virtual void Update() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Event Attributes
		virtual void SetEventCallback(const EventCallBackFn& callback) = 0;

		virtual void* GetNativeWindow() const = 0;

		// Must be implemented per platform
		static Window* Create(const WindowProps& props = WindowProps());
	};

}