#include "Animation.h"

namespace Engine {

	Animation::Animation() : m_Length(0.0f), m_Frames(0), m_NumOfFrames(0) { }

	Animation::Animation(const float& length, const std::vector<KeyFrame>& frames)
	{
		this->m_Length = length;
		this->m_Frames = frames;
		this->m_NumOfFrames = frames.size();
	}
}