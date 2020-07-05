#pragma once

#include "Light.h"

namespace Engine {

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		DirectionalLight(glm::vec3 direction,
			uint32_t shadowWidth,
			uint32_t shadowHeight,
			glm::vec3 ambientColor,
			float ambientIntesnity,
			float diffuseIntensity);
		~DirectionalLight();

		glm::mat4 CalculateLightTransform();

		virtual void SendToShader(Shader& shader) override;

	protected:
		glm::vec3 m_Direction;
	};

}


