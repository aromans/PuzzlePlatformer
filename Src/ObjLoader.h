#pragma once

#include "Vertex.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <map>

namespace Engine {

	static void CalculateNormalMappingInfo(Vertex& v0, Vertex& v1, Vertex& v2, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
	{
		glm::vec3& p0 = v0.position;
		glm::vec3& p1 = v1.position;
		glm::vec3& p2 = v2.position;

		glm::vec2& uv0 = v0.texcoord;
		glm::vec2& uv1 = v1.texcoord;
		glm::vec2& uv2 = v2.texcoord;

		glm::vec3 deltaPos1 = p1 - p0;
		glm::vec3 deltaPos2 = p2 - p0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}

	static void DealWithSet(Vertex& v0, Vertex& v1, Vertex& v2, std::vector<Vertex>& vertices, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
	{
		CalculateNormalMappingInfo(v0, v1, v2, tangents, bitangents);

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}

	static std::tuple<std::vector<Vertex>, std::vector<unsigned int>> LoadOBJ(const char* file_path)
	{
		// Vertex Information
		std::vector<glm::vec3> vertex_positions;
		std::vector<glm::vec2> vertex_texcoords;
		std::vector<glm::vec3> vertex_normals;

		// Normal Mapping Calculations
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;

		// Vertex Array
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// File Parse Information
		std::string line = "";
		std::string prefix = "";
		std::ifstream stream(file_path);

		std::map<Vertex, int> vertex_hash;

		if (!stream.is_open())
		{
			printf("ERROR: Could not open .OBJ file at the path '%s'", file_path);
			return {};
		}

		while (std::getline(stream, line))
		{
			std::istringstream linestream(line);
			linestream >> prefix;

			// Vertex Position Info
			if (prefix == "v")
			{
				glm::vec3 vertex_pos;
				linestream >> vertex_pos.x >> vertex_pos.y >> vertex_pos.z;
				vertex_positions.push_back(vertex_pos);
			}

			// Vertex Texture Coordinate Info
			if (prefix == "vt")
			{
				glm::vec2 vertex_texcoord;
				linestream >> vertex_texcoord.x >> vertex_texcoord.y;
				vertex_texcoords.push_back(vertex_texcoord);
			}

			// Vertex Normal Info
			if (prefix == "vn")
			{
				glm::vec3 vertex_norm;
				linestream >> vertex_norm.x >> vertex_norm.y >> vertex_norm.z;
				vertex_normals.push_back(vertex_norm);
			}

			// Face (Indicies) Info
			if (prefix == "f")
			{
				std::string face = "";
				std::vector<std::tuple<GLint, GLint, GLint>> faces;

				while (linestream >> face)
				{
					std::replace(face.begin(), face.end(), '/', ' ');
					std::istringstream facestream(face);

					GLint vIndex;
					GLint tIndex;
					GLint nIndex;
					facestream >> vIndex >> tIndex >> nIndex;

					faces.push_back(std::make_tuple(vIndex, tIndex, nIndex));
				}

				if (faces.size() >= 4) {
					printf("WARNING! The mesh at filepath '%s' is using quad faces! Try exporting the mesh with triangular faces instead!", file_path);
					return {};
				}

				Vertex v0;
				v0.position = vertex_positions[std::get<0>(faces[0]) - 1];
				v0.texcoord = vertex_texcoords[std::get<1>(faces[0]) - 1];
				v0.normal = vertex_normals[std::get<2>(faces[0]) - 1];
				Vertex v1;
				v1.position = vertex_positions[std::get<0>(faces[1]) - 1];
				v1.texcoord = vertex_texcoords[std::get<1>(faces[1]) - 1];
				v1.normal = vertex_normals[std::get<2>(faces[1]) - 1];
				Vertex v2;
				v2.position = vertex_positions[std::get<0>(faces[2]) - 1];
				v2.texcoord = vertex_texcoords[std::get<1>(faces[2]) - 1];
				v2.normal = vertex_normals[std::get<2>(faces[2]) - 1];

				DealWithSet(v0, v1, v2, vertices, tangents, bitangents);
			}
		}

		stream.close();

		std::vector<glm::vec3> indexed_tangents;
		std::vector<glm::vec3> indexed_bitangents;

		std::vector<Vertex> out_vertices;

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			Vertex v = vertices[i];

			std::map<Vertex, int>::iterator it = vertex_hash.find(v);

			if (it != vertex_hash.end()) {
				unsigned int index = (unsigned int)it->second;
				indices.push_back(index);
				indexed_tangents[index] += tangents[i];
				indexed_bitangents[index] += bitangents[i];
			}
			else {
				out_vertices.push_back(v);
				indexed_tangents.push_back(tangents[i]);
				indexed_bitangents.push_back(bitangents[i]);

				unsigned int new_index = (unsigned int)out_vertices.size() - 1;
				indices.push_back(new_index);
				vertex_hash[v] = new_index;
			}
		}

		for (size_t i = 0; i < out_vertices.size(); ++i) {
			out_vertices[i].averagedTangent = glm::normalize(indexed_tangents[i]);
			out_vertices[i].averagedBitangent = glm::normalize(indexed_bitangents[i]);

			glm::vec3& n = out_vertices[i].normal;
			glm::vec3& t = out_vertices[i].averagedTangent;
			glm::vec3& b = out_vertices[i].averagedBitangent;

			t = glm::normalize(t - glm::dot(t, n) * n);

			if (glm::dot(glm::cross(n, t), b) < 0.0f) {
				t = t * -1.0f;
			}

			b = glm::normalize(glm::cross(t, n));
		}

		printf("OBJ file was successfully loaded! Number of Vertices (%i) and Indices (%i)\n", out_vertices.size(), indices.size());

		return { out_vertices, indices };
	}

}

