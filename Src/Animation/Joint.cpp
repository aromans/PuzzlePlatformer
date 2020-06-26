#include "Joint.h"

namespace Engine {

	Joint::Joint(const int& id, const std::string& name, const glm::mat4& localTransform, const glm::mat4& invBindTransform) {

		this->m_JointId = id;
		this->m_Name = name;

		this->m_SkeletonBindShapeMatrix = (glm::mat4(glm::vec4(1.f, 0.f, 0.f, 4.26326e-14f),
			glm::vec4(0.f, 1.f, 0.f, -1.89182e-13f),
			glm::vec4(0.f, 0.f, 1.f, 4.9738e-14f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)));
			/*glm::mat4(glm::vec4(1.f, 0.f, 0.f, 0.f),
			glm::vec4(0.f, 1.f, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f));*/
			
			
			
		glm::mat4 bind_matrix = glm::mat4(glm::vec4(.01f, 0.f, 0.f, 0.f),
			glm::vec4(0.f, .01f, 0.f, 0.f),
			glm::vec4(0.f, 0.f, .01f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f));
			/*glm::mat4(glm::vec4(1.f, 0.f, 0.f, 0.f),
				glm::vec4(0.f, 1.f, 0.f, 0.f),
				glm::vec4(0.f, 0.f, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f));*/
			
			
		this->m_JointSpaceTransform = localTransform;
		this->m_InverseBindPoseMatrix = invBindTransform;
	}

	Joint::Joint(const int& id, const std::string& name)
	{
		this->m_JointId = id;
		this->m_Name = name;
	}

	// Recursively collect each Joints InverseBindTransform - 
	// - root joint's local position is an identity matrix. 
	// Function is only called once after data collection.
	Joint* Joint::CalcInverseBindTransform(glm::mat4 parentLocalPosition, Joint* j)
	{
		// Position of joint in relation to parent - parent/model space
		m_GlobalPose = m_JointSpaceTransform * parentLocalPosition;

		for (Joint& child : Children) {
			child.CalcInverseBindTransform(m_GlobalPose, &child);
		}

		return j;
	}

	Joint* Joint::CalcAnimatedBindTransform(glm::mat4 globalInverseTransform, Joint* j)
	{
		glm::mat4 finalBindPoseLocation;
		if (j->GetInvBndMatrix() != glm::mat4()) {
			// vCM = Kj * vBM
			finalBindPoseLocation = (j->GetInvBndMatrix() * j->GetGlobalPose() * globalInverseTransform);
			j->SetAnimationTransform((finalBindPoseLocation));
		}

		for (Joint& child : Children) {
			child.CalcAnimatedBindTransform(globalInverseTransform, &child);
		}

		return j;
	}

	Joint* Joint::ApplyPosesToJoints(std::unordered_map<int, glm::mat4> newPose, Joint* j, glm::mat4 parentTransform, glm::mat4 ok)
	{
		glm::mat4 NodeTransformation = j->GetLocalPosition();
		if (newPose.find(j->GetJointId()) != newPose.cend()) {
			NodeTransformation = newPose[j->GetJointId()];
		}

		glm::mat4 GlobalTransformation = (NodeTransformation * parentTransform);

		for (Joint& child : Children) {
			child.ApplyPosesToJoints(newPose, &child, GlobalTransformation, ok);
		}

		if (j->GetInvBndMatrix() != glm::mat4()) {
			glm::mat4 finalBindPoseLocation = glm::transpose(j->GetInvBndMatrix() * GlobalTransformation * ok);
			j->SetAnimationTransform(finalBindPoseLocation);
		}

		return j;
	}

	Joint* Joint::GetJointWithId(const int& id)
	{
		if (this->m_JointId == id) return this;

		Joint* chosen = nullptr;
		for (Joint& child : this->Children) {
			chosen = child.GetJointWithId(id);

			if (chosen) return chosen;
		}

		return nullptr;
	}

}
