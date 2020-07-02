#include "Epch.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>

namespace Engine {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		//auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//auto state = glfwGetKey(window, keycode);
		//return state == GLFW_PRESS || state == GLFW_REPEAT;
		return true;
	}

	bool WindowsInput::IsKeyReleasedImpl(int keycode)
	{
		//auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//auto state = glfwGetKey(window, keycode);
		//return state == GLFW_RELEASE;
		return true;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		//auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//auto state = glfwGetMouseButton(window, button);
		//return state == GLFW_PRESS;
		return true;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		//auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		//double xPos, yPos;
		//glfwGetCursorPos(window, &xPos, &yPos);
		//return { (float)xPos, (float)yPos };
		return {};
	}

}
