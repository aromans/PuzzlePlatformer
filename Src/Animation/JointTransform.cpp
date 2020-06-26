#include "JointTransform.h"

namespace Engine {

	JointTransform::JointTransform(const glm::vec3& position, const glm::quat& rotation)
	{
		this->m_Position = position;
		this->m_Rotation = rotation;
	}

	glm::mat4 JointTransform::getLocalTransform()
	{
		glm::mat4 S = glm::mat4(1.0f);
		glm::mat4 T = (glm::translate(glm::mat4(1.0f), m_Position));
		glm::mat4 R = (glm::toMat4(m_Rotation));
		glm::mat4 final = glm::transpose(T * R * S);
		return final;
	}

	JointTransform JointTransform::Interpolate(JointTransform a, JointTransform b, const float& time)
	{
		glm::vec3 delta = b.m_Position - a.m_Position;
		glm::vec3 newPos = InterpolatePositions(a.m_Position, b.m_Position, time);
		glm::quat newRot = InterpolateRotation(a.m_Rotation, b.m_Rotation, time);
		newRot = glm::normalize(newRot);
		return JointTransform(newPos, newRot);
	}

	glm::vec3 JointTransform::InterpolatePositions(glm::vec3 pos1, glm::vec3 pos2, const float& time)
	{
		return glm::mix(pos1, pos2, time);
	}

	glm::quat JointTransform::InterpolateRotation(glm::quat pStart, glm::quat pEnd, const float& blend)
	{
		return glm::slerp(pStart, pEnd, blend);
	}

}