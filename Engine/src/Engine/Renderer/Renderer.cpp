#include "Epch.h"
#include "Renderer.h"
#include "Engine/Platform/OpenGL/OpenGLRenderer.h"

namespace Engine {

	void Renderer::BeginScene() 
	{
	
	}

	void Renderer::EndScene() 
	{
	
	}

	void Renderer::Submit(const VAO& vertexArray)
	{
		vertexArray.Bind();
		if (vertexArray.GetIBO() != nullptr) {
			vertexArray.GetIBO()->Bind();
			OpenGLRenderer::DrawIndexed(vertexArray);
			vertexArray.GetIBO()->Unbind();
		}
		else {
			OpenGLRenderer::DrawVertices(vertexArray);
		}
		vertexArray.Unbind();
	}

}