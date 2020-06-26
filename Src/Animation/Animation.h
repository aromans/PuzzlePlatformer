#pragma once

#include "KeyFrame.h"
#include <vector>

namespace Engine {

	class Animation
	{
	public:
		Animation();
		Animation(const float& length, const std::vector<KeyFrame>& frames);
		~Animation();

		std::vector<KeyFrame> GetFrames() const { return m_Frames; }
		const float GetLength() const { return m_Length; }
		const int GetTotalFrameAmount() const { return m_NumOfFrames; }

	protected:
		std::vector<KeyFrame> m_Frames;
		float m_Length;
		int m_NumOfFrames;
	};

}