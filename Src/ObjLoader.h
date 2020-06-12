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

static std::tuple<std::vector<Vertex>, std::vector<unsigned int>> LoadOBJ(const char* file_path)
{
	// Vertex Information
	std::vector<glm::vec3> vertex_positions;
	std::vector<glm::vec2> vertex_texcoords;
	std::vector<glm::vec3> vertex_normals;

	// Face Information
	std::vector<std::tuple<GLint, GLint, GLint>> obj_faces;

	// Vertex Array
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// File Parse Information
	std::string line = "";
	std::string prefix = "";
	std::ifstream stream(file_path);

	if (stream.is_open()) 
	{
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

				// Tri 1
				obj_faces.push_back(faces[0]);
				obj_faces.push_back(faces[1]);
				obj_faces.push_back(faces[2]);

				// Quad
				if (faces.size() >= 4) 
				{
					// Tri 2
					obj_faces.push_back(faces[0]);
					obj_faces.push_back(faces[2]);
					obj_faces.push_back(faces[3]);
				}
			}
		}
	}
	else {
		printf("ERROR: Error loading OBJ file, could not find file at the path '%s'", file_path);
		return {};
	}

	stream.close();

	// Reserve size needed for Vertices to build mesh
	//vertices.resize(obj_faces.size(), Vertex());

	try 
	{
		for (const auto& face : obj_faces)
		{
			unsigned int vIndex = std::get<0>(face) - 1;
			indices.push_back(vIndex);

			Vertex v;
			v.color = glm::vec3(1.0f, 1.0f, 1.0f);
			v.position = vertex_positions[vIndex];
			v.texcoord = vertex_texcoords[(std::get<1>(face) - 1)];
			v.normal = vertex_normals[(std::get<2>(face) - 1)];
			vertices.push_back(v);
		}
	}
	catch (const std::exception& e) {
		printf("SOMETHING HAS HAPPENED: '%s'", e.what());
		return {};
	}

	printf("OBJ file was successfully loaded! Number of Vertices (%i) and Indices (%i)", vertices.size(), indices.size());

	return { vertices, indices };
}

