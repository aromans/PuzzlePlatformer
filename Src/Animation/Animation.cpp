#include "Animation.h"

namespace Engine {

	Animation::Animation()
	{

	}

	Animation::Animation(const float& length, const std::vector<KeyFrame>& frames)
	{
		this->m_Length = length;
		this->m_Frames = frames;
		this->m_NumOfFrames = frames.size();
	}

	Animation::~Animation()
	{

	}

}