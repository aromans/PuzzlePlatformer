#pragma once

#include "JointTransform.h"

#include <vector>

#include <unordered_map>

namespace Engine {

	class KeyFrame
	{
	public:
		KeyFrame() : m_Timestamp(0.0f) { }
		KeyFrame(const float& timestamp, const std::unordered_map<int, JointTransform>& transforms)
			: m_Timestamp(timestamp), m_Transforms(transforms) { }
		~KeyFrame() {}

		inline float GetTimeStamp() const { return m_Timestamp; }
		inline std::unordered_map<int, JointTransform> GetJointKeyFrames() const { return m_Transforms; }

	protected:
		std::unordered_map<int, JointTransform> m_Transforms;
		float m_Timestamp;
	};

}

