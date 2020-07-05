#include "Epch.h"
#include "Application.h"

#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		m_Window = Window::Create(WindowProps("Puzzle Platformer", 1366, 768));
		m_ActiveScene = std::make_unique<Scene>();
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Init()
	{
		// framebuffer configuration
		// -------------------------
		FrameBufferElement fbElement;
		fbElement.Width = 1366;
		fbElement.Height = 768;
		m_FrameBuffer = FrameBuffer::Create(fbElement);
		m_FrameBuffer->Bind();

		// Calculate Projection Matrix
		GLfloat aspectRatio = (GLfloat)m_FrameBuffer->GetElement().Width / (GLfloat)m_FrameBuffer->GetElement().Height;
		m_Projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f);
		m_Orthographic = glm::ortho(-aspectRatio, aspectRatio, -20.0f, 20.0f, 0.01f, 100.0f);
	}

	void Application::OnRender()
	{
		// Clear window
		m_FrameBuffer->Unbind();
		glClearColor(0.15f, 0.11f, 0.19f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Renderer::BeginScene();
		{
			m_FrameBuffer->Bind();
			// Scene Stuff Here . . .
			Renderer::EndScene();
		}

		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Application::Run()
	{
		Init();	 // Engine Initialization
		Awake(); // Application Initialization
		while (m_IsRunning)
		{
			float time = m_Window->GetTime();
			m_CurrentTime = time - m_LastTime;
			if (m_CurrentTime > 0.25)
				m_CurrentTime = 0.25;
			m_LastTime = time;

			OnUpdate(m_CurrentTime);

			OnRender();

			m_Window->Update();
		}
		OnShutdown();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}
}