#pragma once

#include "Epch.h"

namespace Engine {

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

	struct AnimVertex
	{
		glm::vec3 position;
		glm::vec2 texcoord;
		glm::vec3 normal;
		glm::vec3 averagedTangent = glm::vec3(0.0f);
		glm::vec3 averagedBitangent = glm::vec3(0.0f);
		glm::ivec4 jointIds;
		glm::vec4 weights;

		bool operator<(const AnimVertex that) const {
			return memcmp((void*)this, (void*)&that, sizeof(AnimVertex)) > 0;
		}
	};

}