#pragma once

#include <glm\glm.hpp>

#include <vector>

struct Joint
{
	int JointId;
	glm::mat4 Transform;
	std::vector<Joint> Children;
};