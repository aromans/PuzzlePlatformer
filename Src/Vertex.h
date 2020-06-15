#pragma once

#include <glm\glm.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec3 averagedTangent = glm::vec3(0.0f);
	glm::vec3 averagedBitangent = glm::vec3(0.0f);

	bool operator<(const Vertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(Vertex)) > 0;
	}
};