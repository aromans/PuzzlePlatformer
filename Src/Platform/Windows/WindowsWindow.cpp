#include "WindowsWindow.h"
#include "src/Events/ApplicationEvent.h"
#include "src/Events/KeyEvent.h"
#include "src/Events/MouseEvent.h"

namespace Engine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		printf("GLFW error (%s): %s", error, description);
	}

	Window* Window::Create(const WindowProps& data)
	{
		return new WindowsWindow(data);
	}

	WindowsWindow::WindowsWindow(const WindowProps& data) {
		Init(data);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProps& data)
	{
		m_Data.Title = data.Title;
		m_Data.Width = data.Width;
		m_Data.Height = data.Height;

		// Initialize GLFW
		if (!glfwInit()) {
			printf("GLFW Initialization failed!");
			glfwTerminate();
			return;
		}

		s_GLFWInitialized = true;

		// Setup GLFW window properties
		// OpenGL Version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // No Backwards Compatibility
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Allow Forwards Compatibility

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

		if (!m_Window) {
			printf("GLFW window creation failed!");
			glfwTerminate();
			return;
		}

		// Set context for GLEW to use
		glfwMakeContextCurrent(m_Window);

		// Assigns this window for listening to user input
		glfwSetWindowUserPointer(m_Window, &m_Data);

		// Set GLFW event callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent windowEvent(width, height);
			data.EventCallBack(windowEvent);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent windowEvent;
			data.EventCallBack(windowEvent);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int code, int action, int mode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent keyEvent(key, 0);
				data.EventCallBack(keyEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent keyEvent(key);
				data.EventCallBack(keyEvent);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyTypedEvent keyEvent(key);
				data.EventCallBack(keyEvent);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int key)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent charEvent(key);
			data.EventCallBack(charEvent);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent mouseEvent(button);
				data.EventCallBack(mouseEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent mouseEvent(button);
				data.EventCallBack(mouseEvent);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent mouseEvent((float)xOffset, (float)yOffset);
			data.EventCallBack(mouseEvent);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent mouseEvent((float)xPos, (float)yPos);
			data.EventCallBack(mouseEvent);
		});
	}

	void WindowsWindow::Update()
	{
		// Get + Handle user input events
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

}