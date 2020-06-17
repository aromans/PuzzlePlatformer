#pragma once

#include "PointLight.h"

namespace Engine {

	class SpotLight :
		public PointLight
	{
	public:
		SpotLight();
		SpotLight(glm::vec3 direction, GLfloat edge,
			glm::vec3 position, GLfloat con, GLfloat lin, GLfloat exp,
			glm::vec3 ambientColor, GLfloat ambientIntesnity, GLfloat diffuseIntensity);
		~SpotLight();

		void SetFlash(const glm::vec3& pos, const glm::vec3& dir);

		virtual void SendToShader(Shader& shader, const int& i);
		virtual void SendToShader(Shader& shader);

	private:
		glm::vec3 m_Direction;
		GLfloat m_Edge;
		GLfloat m_ProcEdge;	// Processed Edge 
	};

}

