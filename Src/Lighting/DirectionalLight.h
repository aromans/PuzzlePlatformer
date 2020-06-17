#pragma once

#include "Light.h"

namespace Engine {

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		DirectionalLight(glm::vec3 direction,
			GLuint shadowWidth,
			GLuint shadowHeight,
			glm::vec3 ambientColor,
			GLfloat ambientIntesnity,
			GLfloat diffuseIntensity);
		~DirectionalLight();

		glm::mat4 CalculateLightTransform();

		virtual void SendToShader(Shader& shader) override;

	protected:
		glm::vec3 m_Direction;
	};

}


