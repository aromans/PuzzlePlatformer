#pragma once

#include "src/Window.h"

#include <GLFW/glfw3.h>

namespace Engine {

	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& data);
		virtual ~WindowsWindow();

		void Update() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window Attributes
		inline void SetEventCallback(const EventCallBackFn& callback) override { m_Data.EventCallBack = callback; }

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& data);
		virtual void ShutDown();

	private:
		GLFWwindow* m_Window;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;

			EventCallBackFn EventCallBack;
		};

		WindowData m_Data;
	};

}