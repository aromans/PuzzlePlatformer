#include "Renderer.h"
#include "./OpenGL/OpenGLRenderer.h"

namespace Engine {

	void Renderer::BeginScene() {}
	void Renderer::EndScene() {}

	void Renderer::Submit(const VAO& vertexArray)
	{
		vertexArray.Bind();
		vertexArray.GetIBO()->Bind();
		OpenGLRenderer::DrawIndexed(vertexArray);
		vertexArray.GetIBO()->Unbind();
		vertexArray.Unbind();
	}

}