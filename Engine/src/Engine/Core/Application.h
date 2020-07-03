#pragma once

#include <memory>

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Core/Events/ApplicationEvent.h"

namespace Engine {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		virtual void OnInit();
		virtual void OnShutdown() {}
		virtual void OnUpdate(double& dt) {}
		virtual void OnRender();

		virtual void OnEvent(Event& e);

		inline Window& GetWindow() { return *m_Window; }
		static inline Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;

		double m_CurrentTime;
		double m_LastTime;

		glm::vec2 m_Viewport = glm::vec2(0.0f);

		glm::mat4 m_Projection;
		glm::mat4 m_Orthographic;

		std::shared_ptr<FrameBuffer> m_FrameBuffer;

		bool m_IsRunning = true;
		static Application* s_Instance;
	};

	// Game Call
	Application* CreateApplication();
}
