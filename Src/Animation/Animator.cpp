#include "Animator.h"

namespace Engine {

	Animator::Animator(const std::string& file_path)
	{
		auto ParsedAnimationData = LoadSkeletonAndAnimations(file_path.c_str());
		m_AnimationList = std::get<0>(ParsedAnimationData);
		m_Skeleton = std::get<1>(ParsedAnimationData);
		m_JointNames = std::get<2>(ParsedAnimationData);

		m_Skeleton.CalcInverseBindTransform(glm::mat4(1.0f), &m_Skeleton);
		rootGlobalInverseTransform = glm::inverse(m_Skeleton.GetLocalPosition());
		m_Skeleton.CalcAnimatedBindTransform(rootGlobalInverseTransform, &m_Skeleton);
	}

	Animator::~Animator()
	{
	}

	void Animator::AddAnimation(const std::string& obj_path, const std::string& anim_name)
	{
		Animation newAnim = LoadAnimation(obj_path.c_str(), m_Skeleton, m_JointNames);

		if (newAnim.GetFrames().size() < 0) { printf("ERROR! Something went wrong parsing animation at path '%s'", obj_path); }

		m_AnimationList[anim_name] = newAnim;
	}

	void Animator::Play(const std::string& animation_name)
	{
		if (m_NameHash == std::hash<std::string>{}(animation_name)) return;

		if (m_AnimationList.find(animation_name) != m_AnimationList.cend()) {
			if (IsPlaying()) Stop();

			m_CurrentAnimation = m_AnimationList[animation_name];
			m_TotalAnimationLength = m_CurrentAnimation.GetLength();
			m_CurrentAnimationFrameSet = m_CurrentAnimation.GetFrames();
			m_AnimationTime = 0.0f;
			m_CurrentFrameIndex = 0;
			m_IsPlaying = true;
			m_NameHash = std::hash<std::string>{}(animation_name);
		}
	}

	void Animator::Stop()
	{
		m_IsPlaying = false;
		m_AnimationTime = 0.0;
		m_CurrentFrameIndex = 0;
		m_NameHash = 0;
	}

	void Animator::Update(double& dt)
	{
		if (IsPlaying()) {

			IncreaseAnimationTime(dt);

			std::unordered_map<int, glm::mat4> currentPose = CalculateCurrentAnimationPose();
			m_Skeleton.ApplyPosesToJoints(currentPose, &m_Skeleton, glm::mat4(1.0f), rootGlobalInverseTransform);
		}
	}

#pragma region
	void Animator::IncreaseAnimationTime(double& dt)
	{
		m_AnimationTime += dt;	// Time elapsed since start
		if (m_AnimationTime > m_TotalAnimationLength) {
			m_AnimationTime = 0.0f;
		}
	}

	void Animator::GetPreviousAndNextFrame(float& time)
	{
		for (int i = 0; i < m_CurrentAnimationFrameSet.size(); ++i) {

			if (time < (float)m_CurrentAnimationFrameSet[i + 1].GetTimeStamp()) {
				m_PreviousFrame = i;
				m_NextFrame = i + 1;
				return;
			}
		}
	}

	float Animator::GetAnimationProgress()
	{
		float frameLength = m_CurrentAnimationFrameSet[m_NextFrame].GetTimeStamp() - m_CurrentAnimationFrameSet[m_PreviousFrame].GetTimeStamp();
		float timeElapsed = m_AnimationTime - m_CurrentAnimationFrameSet[m_PreviousFrame].GetTimeStamp();
		return timeElapsed / frameLength;
	}

	std::unordered_map<int, glm::mat4> Animator::InterpolatePoses(float time) {

		std::unordered_map<int, glm::mat4> poses;

		KeyFrame prev = m_CurrentAnimationFrameSet[m_PreviousFrame];
		KeyFrame next = m_CurrentAnimationFrameSet[m_NextFrame];

		for (std::pair<int, JointTransform> keyframe : prev.GetJointKeyFrames()) {

			JointTransform previousFrame = prev.GetJointKeyFrames()[keyframe.first];
			JointTransform nextFrame = next.GetJointKeyFrames()[keyframe.first];

			JointTransform interpolated = JointTransform::Interpolate(previousFrame, nextFrame, time);
			poses[keyframe.first] = (interpolated.getLocalTransform());
		}
		return poses;
	}

	std::unordered_map<int, glm::mat4> Animator::CalculateCurrentAnimationPose()
	{
		GetPreviousAndNextFrame(m_AnimationTime);
		float time = GetAnimationProgress();
		return InterpolatePoses(time);
	}
#pragma endregion
}