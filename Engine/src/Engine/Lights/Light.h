#pragma once

#include "Epch.h"
#include "Engine/Renderer/Shader.h"

namespace Engine {

	class Light {
	public:
		Light()
		{
			m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
			m_AmbientIntensity = 1.0f;
			m_DiffuseIntensity = 1.0f;
		}

		Light(uint32_t shadowWidth, uint32_t shadowHeight,
			glm::vec3 ambientColor, float ambientIntesnity, float diffuseIntensity)
		{
			//m_ShadowMap = new ShadowMap();
			//m_ShadowMap->Init(shadowWidth, shadowHeight);

			m_Color = ambientColor;
			m_AmbientIntensity = ambientIntesnity;
			m_DiffuseIntensity = diffuseIntensity;
		}

		~Light() {}

		//ShadowMap* GetShadowMap() const { return m_ShadowMap; }

		virtual void SendToShader(Shader& shader) = 0;

	protected:
		glm::vec3 m_Color;
		float m_AmbientIntensity;
		float m_DiffuseIntensity;

		glm::mat4 m_LightProj;

		//ShadowMap* m_ShadowMap;
	};

}