#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

namespace Engine {

	class JointTransform
	{
	public:
		JointTransform() : m_Position(glm::vec3()), m_Rotation(glm::quat()) { }
		JointTransform(const glm::vec3& position, const glm::quat& rotation);
		~JointTransform() {};

		glm::mat4 getLocalTransform();

		static JointTransform Interpolate(JointTransform a, JointTransform b, const float& time);

	private:
		static glm::vec3 InterpolatePositions(glm::vec3 pos1, glm::vec3 pos2, const float& time);
		static glm::quat InterpolateRotation(glm::quat rot1, glm::quat rot2, const float& time);

	protected:
		glm::vec3 m_Position;
		glm::quat m_Rotation;
	};

}