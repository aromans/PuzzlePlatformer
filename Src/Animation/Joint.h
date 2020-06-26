#pragma once

#include <vector>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <map>
#include <unordered_map>

namespace Engine {

	class Joint
	{
	public:
		Joint() {}
		Joint(const int& id, const std::string& name, const glm::mat4& localTransform, const glm::mat4& inverseBindTransform);
		Joint(const int& id, const std::string& name);

		Joint AddChild(const Joint& child) { 
			this->Children.push_back(child); 
			return this->Children.back();
		}

		Joint* CalcInverseBindTransform(glm::mat4 parentBindTransform, Joint* j);
		Joint* CalcAnimatedBindTransform(glm::mat4 identity, Joint* j);


		int GetJointId() const { return m_JointId; }

		glm::mat4 GetLocalPosition() const { return m_JointSpaceTransform; }

		void SetOffsetMatrix(const glm::mat4 offsetMatrix) { this->m_offsetMatrix = offsetMatrix; }
		glm::mat4 GetOffsetMatrix() const { return m_offsetMatrix; }

		void SetGlobalPose(const glm::mat4& globalPose) { this->m_GlobalPose = globalPose; }
		glm::mat4 GetGlobalPose() const { return m_GlobalPose; }

		void SetInvBndMatrix(const glm::mat4& invBind) { this->m_InverseBindPoseMatrix = invBind; }
		glm::mat4 GetInvBndMatrix() const { return m_InverseBindPoseMatrix; }
		void SetAnimationTransform(const glm::mat4& animationTransform) { this->m_CurrentAnimationTransform = animationTransform; }
		glm::mat4 GetAnimatedTransform() const { return m_CurrentAnimationTransform; }

		glm::mat4 GetSkeletonBindShapeMatrix() const { return m_SkeletonBindShapeMatrix; }

		std::vector<Joint> GetChildren() const { return Children; }
		std::string GetName() const { return m_Name;}

		Joint* ApplyPosesToJoints(std::unordered_map<int, glm::mat4> newPose, Joint* j, glm::mat4 parentTransform, glm::mat4 ok);

		Joint* GetJointWithId(const int& id);

	protected:
		int m_JointId;
		std::string m_Name;

		std::vector<Joint> Children;

		glm::mat4 m_CurrentAnimationTransform;

		glm::mat4 m_JointSpaceTransform;

		glm::mat4 m_GeometryWorldSpaceTransform;

		glm::mat4 m_SkeletonBindShapeMatrix;

		glm::mat4 m_GlobalPose;

		glm::mat4 m_offsetMatrix;

		glm::mat4 m_InverseBindPoseMatrix;
	};

}