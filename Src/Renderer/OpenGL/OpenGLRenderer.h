#pragma once

#include "../VAO.h"

#include <memory>
#include <glm/glm.hpp>
#include <GLFW\glfw3.h>

struct GLFWwindow;
struct VAO;

namespace Engine {

	class OpenGLRenderer
	{
	public:
		OpenGLRenderer(GLFWwindow* window);

		void Init();
		void SwapBuffers();
		void SetClearColor(const glm::vec4& color);
		void Clear();

		inline static void DrawIndexed(const VAO& vertexArray) {
			glDrawElements(GL_TRIANGLES, vertexArray.GetIBO()->GetCount(), GL_UNSIGNED_INT, (void*)0);
		}

		inline static void DrawVertices(const VAO& vertexArray) {
			glDrawArrays(GL_TRIANGLES, 0, vertexArray.GetVBOs()[0]->GetCount());
		}

		GLFWwindow* GetWindow() const { return m_WindowHandle; }

	private:
		GLFWwindow* m_WindowHandle;
	};

}

