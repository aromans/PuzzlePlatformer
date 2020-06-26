#pragma once

#include "Animation.h"
#include "Joint.h"
#include "KeyFrame.h"
#include "DaeLoader.h"

#include <unordered_map>
#include <glm\glm.hpp>

namespace Engine {

	class Animator
	{
	public:
		Animator(const std::string& file_path);
		~Animator();

		void AddAnimation(const std::string& file_path, const std::string& anim_name);

		void Play(const std::string& animation_name);
		void Stop();

		void Update(double& dt);

		inline bool IsPlaying() const { return m_IsPlaying; }

		const Joint& GetSkeletonRoot() const { return m_Skeleton; }

	private:

		void IncreaseAnimationTime(double& dt);
		void GetPreviousAndNextFrame(float& time);
		float GetAnimationProgress();
		std::unordered_map<int, glm::mat4> InterpolatePoses(float time);
		std::unordered_map<int, glm::mat4> CalculateCurrentAnimationPose();

	private:
		std::unordered_map<std::string, Animation> m_AnimationList;

		Animation m_CurrentAnimation;

		uint32_t m_NextFrame;
		uint32_t m_PreviousFrame;

		size_t m_NameHash;	// Hash of the current animation playing

		Joint m_Skeleton;

		std::vector<std::string> m_JointNames;
		std::vector<KeyFrame> m_CurrentAnimationFrameSet;

		glm::mat4 rootGlobalInverseTransform;

		float m_AnimationTime;
		float m_TotalAnimationLength;

		bool m_IsPlaying;

		int m_CurrentFrameIndex;
	};
}


