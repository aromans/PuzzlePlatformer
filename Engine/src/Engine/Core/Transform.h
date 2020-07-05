#pragma once

#include "Epch.h"

namespace Engine {

	class Transform {
	public:
		Transform(glm::vec3 pos) 
			: m_Position(pos), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)) { }
		Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
			: m_Position(pos), m_Rotation(rot), m_Scale(scale) { }

		inline glm::mat4 GetWorldTransform() {
			glm::mat4 T = glm::translate(glm::mat4(1.0f), m_Position);
			glm::quat quatRotation = glm::quat(m_Rotation);
			glm::mat4 R = glm::mat4_cast(quatRotation);
			glm::mat4 S = glm::scale(glm::mat4(1.0f), m_Scale);

			return T * R * S;
		}

		inline void SetPosition(glm::vec3 pos) { m_Position = pos; }
		inline void SetRotation(glm::vec3 rot) { m_Rotation = rot; }
		inline void SetScale(glm::vec3 scale) { m_Scale = scale; }

		inline glm::vec3 GetPosition() const { return m_Position; }
		inline glm::vec3 GetRotation() const { return m_Rotation; }
		inline glm::vec3 GetSize() const { return m_Scale; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};

}