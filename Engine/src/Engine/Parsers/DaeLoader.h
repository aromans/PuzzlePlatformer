//#pragma once
//
//#include "Epch.h"
//
//#include "Engine/Core/Vertex.h"
//#include "Engine/Parsers/pugixml-1.10/pugiconfig.hpp"
//#include "Engine/Parsers/pugixml-1.10/pugixml.hpp"
//#include "Engine/Parsers/ObjLoader.h"
//
//#include "Engine/Animations/Animation.h"
//#include "Engine/Animations/Joint.h"
//
////////////////////////////////////////////////////////////////////////////
////// THIS FILE IS DIRTY - NEEDS REFACTORING . . .					   ///
////////////////////////////////////////////////////////////////////////////
//
//namespace Engine {
//
//	static void CalculateNormalMappingInfo(AnimVertex& v0, AnimVertex& v1, AnimVertex& v2, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
//	{
//		glm::vec3& p0 = v0.position;
//		glm::vec3& p1 = v1.position;
//		glm::vec3& p2 = v2.position;
//
//		glm::vec2& uv0 = v0.texcoord;
//		glm::vec2& uv1 = v1.texcoord;
//		glm::vec2& uv2 = v2.texcoord;
//
//		glm::vec3 deltaPos1 = p1 - p0;
//		glm::vec3 deltaPos2 = p2 - p0;
//
//		glm::vec2 deltaUV1 = uv1 - uv0;
//		glm::vec2 deltaUV2 = uv2 - uv0;
//
//		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
//		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
//		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
//
//		tangents.push_back(tangent);
//		tangents.push_back(tangent);
//		tangents.push_back(tangent);
//		bitangents.push_back(bitangent);
//		bitangents.push_back(bitangent);
//		bitangents.push_back(bitangent);
//	}
//
//	static void DealWithSet(AnimVertex& v0, AnimVertex& v1, AnimVertex& v2, std::vector<AnimVertex>& vertices, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
//	{
//		CalculateNormalMappingInfo(v0, v1, v2, tangents, bitangents);
//
//		vertices.push_back(v0);
//		vertices.push_back(v1);
//		vertices.push_back(v2);
//	}
//
//	static glm::mat4 GetLocalBindTransform(pugi::xml_node hierarchy)
//	{
//		std::istringstream localBindPose(hierarchy.first_child().first_child().value());
//		glm::mat4 localBindTransform;
//		for (int i = 0; i < 4; ++i) {
//			glm::vec4 row;
//			localBindPose >> row.x >> row.y >> row.z >> row.w;
//			localBindTransform[i] = row;
//		}
//
//		return (localBindTransform);
//	}
//
//	static Joint AddJointHierarchy(Joint& root, const std::vector<std::string>& jointNames, const std::vector<glm::mat4> localMats, const std::vector<glm::mat4> invBindMats, pugi::xml_node currNode, int& index)
//	{
//		for (pugi::xml_node child : currNode.children()) {
//
//			if (!strcmp(child.name(), "node")) {
//				std::string name = child.attribute("name").value();
//
//				glm::mat4 jointSpaceTransform = GetLocalBindTransform(child);
//
//				glm::mat4 invBindTransform;
//				std::vector<std::string>::const_iterator it = std::find(jointNames.cbegin(), jointNames.cend(), name);
//				if (it != jointNames.end()) {
//					int i = std::distance(jointNames.cbegin(), it);
//					invBindTransform = invBindMats[i];
//				} else {
//					invBindTransform = glm::mat4();
//				}
//
//				Joint nextJoint = Joint(index++, name, jointSpaceTransform, invBindTransform);
//				nextJoint = AddJointHierarchy(nextJoint, jointNames, localMats, invBindMats, child, index);
//				root.AddChild(nextJoint);
//			}
//		}
//
//		return root;
//	}
//
//	static int FindJointId(const std::string& name, Joint& j) {
//
//		if (j.GetName() == name) {
//			return j.GetJointId();
//		}
//
//		int id = -1;
//		for (Joint& child : j.GetChildren()) {
//			id = FindJointId(name, child);
//
//			if (id > -1) break;
//		}
//
//		return id;
//	}
//
//	struct search_id_and_name
//	{
//		const char* key;
//		search_id_and_name(const char* k) : key(k) {}
//
//		bool operator()(pugi::xml_node node) const
//		{
//			return !strcmp(node.attribute("id").value(), key) || !strcmp(node.name(), key);
//		}
//	};
//
//	struct search_attribute_value
//	{
//		const char* key;
//		const char* value;
//		search_attribute_value(const char* k, const char* v) : key(k), value(v) {}
//
//		bool operator()(pugi::xml_node node) const
//		{
//			return !strcmp(node.attribute(key).value(), value);
//		}
//	};
//
//	static std::vector<glm::mat4> GetJointInvBindMatrices(pugi::xml_node controllers) 
//	{
//		std::string skin_id = controllers.child("controller").attribute("id").value();
//
//		std::vector<glm::mat4> jointInvBindMatrices;
//		pugi::xml_node invBindMatrixNode = controllers.find_node(search_id_and_name((skin_id + "-bind_poses-array").c_str()));
//		std::istringstream invBindMatrixStream(invBindMatrixNode.first_child().value());
//		while (!invBindMatrixStream.eof()) {
//
//			int counter = 0;
//			glm::mat4 matrix;
//
//			while (counter < 4) {
//				glm::vec4 row;
//				invBindMatrixStream >> row[0] >> row[1] >> row[2] >> row[3];
//				matrix[counter] = row;
//				counter++;
//			}
//
//			jointInvBindMatrices.push_back(matrix);
//		}
//
//		return jointInvBindMatrices;
//	}
//
//	static std::tuple<Joint, std::vector<std::string>> LoadSkeleton(const char* file_path)
//	{
//		std::vector<std::string> jointNames;
//		std::vector<glm::mat4> jointInvBindMatrices;
//
//		pugi::xml_document doc;
//
//		if (!doc.load_file(file_path)) {
//			printf("ERROR! Could not open .DAE file at the path '%s'", file_path);
//			return {};
//		}
//
//		pugi::xml_node controllers = doc.first_child().child("library_controllers");
//
//		std::string skin_id = controllers.child("controller").attribute("id").value();
//
//		pugi::xml_node jointNamesNode = controllers.find_node(search_id_and_name((skin_id + "-joints-array").c_str()));
//		std::istringstream jointNamesStream(jointNamesNode.first_child().value());
//		for (int i = 0; i < std::stoi(jointNamesNode.attribute("count").value()); ++i)
//		{
//			std::string name;
//			jointNamesStream >> name;
//			jointNames.push_back(name);
//		}
//
//		jointInvBindMatrices = GetJointInvBindMatrices(controllers);
//
//		////////////// JOINT HIERARCHY //////////////
//
//		pugi::xml_node visualScene = doc.first_child().child("library_visual_scenes").child("visual_scene");
//
//		std::string root_name = visualScene.attribute("id").value();
//
//		glm::mat4 identity = glm::mat4(1.0f);
//
//		glm::mat4 Y_UP = glm::mat4(0, -1, 0, 0,
//								   1, 0, 0, 0,
//								   0, 0, 1, 0,
//								   0, 0, 0, 1);
//
//		glm::mat4 Z_UP = glm::mat4(1, 0, 0, 0,
//								   0, 0, 1, 0,
//								   0, -1, 0, 0,
//								   0, 0, 0, 1);
//
//		identity *= Y_UP;
//
//		Joint root = Joint(0, root_name.c_str(), identity, glm::mat4());
//
//		int index = 1;
//		root = AddJointHierarchy(root, jointNames, jointInvBindMatrices, jointInvBindMatrices, visualScene, index);
//
//		return { root, jointNames };
//	}
//
//	static std::tuple<std::vector<AnimVertex>, std::vector<unsigned int>> LoadModel(const char* file_path)
//	{
//		pugi::xml_document doc;
//		if (!doc.load_file(file_path)) {
//			printf("ERROR! Could not open .DAE file at the path '%s'", file_path);
//			return {};
//		}
//
//		// Weights Calculations		
//		std::vector<int> counts;
//		std::vector<int> data;
//		std::vector<float> weights_data;
//
//		// Skin/Joint Arrays
//		std::vector<glm::ivec4> jointIds;
//		std::vector<glm::vec4> weightsForJoints;
//
//		pugi::xml_node controllers = doc.first_child().child("library_controllers");
//		std::string skin_id = controllers.child("controller").attribute("id").value();
//
//		auto weightAmnt_array = controllers.find_node(search_id_and_name("vcount")).first_child().value();
//		std::istringstream stream_weights(weightAmnt_array);
//		for (std::string s; stream_weights >> s; counts.push_back(std::stoi(s))) {}
//
//		auto raw_data = controllers.find_node(search_id_and_name("v")).first_child().value();
//		std::istringstream stream_data(raw_data);
//		for (std::string s; stream_data >> s; data.push_back(std::stoi(s))) {}
//
//		auto weights_array = controllers.find_node(search_attribute_value("id", (skin_id + "-weights-array").c_str())).first_child().value();
//		std::istringstream stream_w_values(weights_array);
//		for (std::string s; stream_w_values >> s; weights_data.push_back(std::stof(s))) {}
//
//		const int max = 4;
//		int iiii = 0;
//		int counter = 0;
//		for (auto count : counts) {
//
//			std::vector<int> jntsAffected;
//			std::vector<float> weights;
//
//			for (int i = 0; i < count; ++i) {
//				int jointIndex = data[counter++];
//				float weight = weights_data[data[counter++]];
//
//				jntsAffected.push_back(jointIndex);
//				weights.push_back(weight);
//			}
//
//			if (weights.size() < max) {
//
//				while (jntsAffected.size() < max) {
//
//					jntsAffected.push_back(0);
//					weights.push_back(0.f);
//
//				}
//			}
//
//			glm::ivec4 jid = { jntsAffected[0], jntsAffected[1], jntsAffected[2] , jntsAffected[3] };
//			glm::vec4 w = { weights[0], weights[1], weights[2] , weights[3] };
//
//			if (glm::length(w) - 1.0f >= 0.00000001f) {
//				printf("You gonna have an issue here . . . (%i) + (%f, %f, %f, %f)\n", iiii, w[0], w[1], w[2], w[3]);
//			}
//
//			jointIds.push_back(jid);
//			weightsForJoints.push_back(w);
//
//			iiii++;
//		}
//
//		// Vertex Information
//		std::vector<std::tuple<glm::vec3, glm::ivec4, glm::vec4>> vertex_positions;
//		std::vector<glm::vec2> vertex_texcoords;
//		std::vector<glm::vec3> vertex_normals;
//
//		// Normal Mapping Calculations
//		std::vector<glm::vec3> tangents;
//		std::vector<glm::vec3> bitangents;
//
//		// Vertex Array
//		std::vector<AnimVertex> vertices;
//		std::vector<unsigned int> indices;
//
//		// File Parse Information
//		std::string line;
//		std::string prefix = "";
//		std::ifstream stream(file_path);
//
//		std::map<AnimVertex, int> vertex_hash;
//
//		pugi::xml_node geom = doc.first_child();
//
//		std::string geometry_id = "";
//
//		pugi::xml_node child = geom.child("library_geometries").child("geometry");
//
//		geometry_id = child.attribute("id").value();
//
//		pugi::xml_node vArray = doc.find_node(search_id_and_name((geometry_id + "-positions-array").c_str()));
//
//		std::istringstream vStream(vArray.first_child().value());
//		for (int i = 0; i < std::stoi(vArray.attribute("count").value()); i += 3) {
//			glm::vec3 vertex_pos;
//			vStream >> vertex_pos.x >> vertex_pos.y >> vertex_pos.z;
//
//			int weightAndJointIndex = (i / 3);
//			glm::ivec4 idsForVertex = jointIds[weightAndJointIndex];
//			glm::vec4 weightsForVertex = weightsForJoints[weightAndJointIndex];
//
//			vertex_positions.push_back(std::make_tuple(vertex_pos, idsForVertex, weightsForVertex));
//		}
//
//		pugi::xml_node nArray = doc.find_node(search_id_and_name((geometry_id + "-normals-array").c_str()));
//
//		std::istringstream nStream(nArray.first_child().value());
//		for (int i = 0; i < std::stoi(nArray.attribute("count").value()); i += 3) {
//			glm::vec3 vertex_norm;
//			nStream >> vertex_norm.x >> vertex_norm.y >> vertex_norm.z;
//			vertex_normals.push_back(vertex_norm);
//		}
//
//		pugi::xml_node tArray = doc.find_node(search_id_and_name((geometry_id + "-map-0-array").c_str()));
//
//		std::istringstream tStream(tArray.first_child().value());
//		for (int i = 0; i < std::stoi(tArray.attribute("count").value()); i += 2) {
//			glm::vec2 vertex_texcoord;
//			tStream >> vertex_texcoord.x >> vertex_texcoord.y;
//			vertex_texcoords.push_back(vertex_texcoord);
//		}
//
//		// Faces Calculations
//		pugi::xml_node fArray = doc.find_node(search_id_and_name("triangles"));
//		pugi::xml_node faceIndices = doc.find_node(search_id_and_name("p"));
//
//		std::istringstream fStream(faceIndices.first_child().value());
//		for (int i = 0; i < std::stoi(fArray.attribute("count").value()); ++i) {
//			GLint vIndex0; GLint nIndex0; GLint tIndex0;
//			GLint vIndex1; GLint nIndex1; GLint tIndex1;
//			GLint vIndex2; GLint nIndex2; GLint tIndex2;
//			fStream >> vIndex0 >> nIndex0 >> tIndex0
//				>> vIndex1 >> nIndex1 >> tIndex1
//				>> vIndex2 >> nIndex2 >> tIndex2;
//
//			AnimVertex v0;
//			v0.position = std::get<0>(vertex_positions[vIndex0]);
//			v0.jointIds = std::get<1>(vertex_positions[vIndex0]);
//			v0.weights = std::get<2>(vertex_positions[vIndex0]); 
//			v0.normal = vertex_normals[nIndex0];
//			v0.texcoord = vertex_texcoords[tIndex0];
//			AnimVertex v1;
//			v1.position = std::get<0>(vertex_positions[vIndex1]);
//			v1.jointIds = std::get<1>(vertex_positions[vIndex1]);
//			v1.weights = std::get<2>(vertex_positions[vIndex1]);
//			v1.normal = vertex_normals[nIndex1];
//			v1.texcoord = vertex_texcoords[tIndex1];
//			AnimVertex v2;
//			v2.position = std::get<0>(vertex_positions[vIndex2]);
//			v2.jointIds = std::get<1>(vertex_positions[vIndex2]);
//			v2.weights = std::get<2>(vertex_positions[vIndex2]);
//			v2.normal = vertex_normals[nIndex2];
//			v2.texcoord = vertex_texcoords[tIndex2];
//
//			DealWithSet(v0, v1, v2, vertices, tangents, bitangents);
//		}
//
//
//		std::vector<glm::vec3> indexed_tangents;
//		std::vector<glm::vec3> indexed_bitangents;
//
//		std::vector<AnimVertex> out_vertices;
//
//		stream.close();
//
//		// Add Vertices/Indices 
//		for (size_t i = 0; i < vertices.size(); ++i)
//		{
//			AnimVertex v = vertices[i];
//
//			std::map<AnimVertex, int>::iterator it = vertex_hash.find(v);
//
//			if (it != vertex_hash.end()) {
//				unsigned int index = (unsigned int)it->second;
//				indices.push_back(index);
//				indexed_tangents[index] += tangents[i];
//				indexed_bitangents[index] += bitangents[i];
//			}
//			else 
//			{
//				out_vertices.push_back(v);
//				indexed_tangents.push_back(tangents[i]);
//				indexed_bitangents.push_back(bitangents[i]);
//
//				unsigned int new_index = (unsigned int)out_vertices.size() - 1;
//				indices.push_back(new_index);
//				vertex_hash[v] = new_index;
//			}
//		}
//
//		for (size_t i = 0; i < out_vertices.size(); ++i) {
//			out_vertices[i].averagedTangent = glm::normalize(indexed_tangents[i]);
//			out_vertices[i].averagedBitangent = glm::normalize(indexed_bitangents[i]);
//
//			glm::vec3& n = out_vertices[i].normal;
//			glm::vec3& t = out_vertices[i].averagedTangent;
//			glm::vec3& b = out_vertices[i].averagedBitangent;
//
//			t = glm::normalize(t - glm::dot(t, n) * n);
//
//			if (glm::dot(glm::cross(n, t), b) < 0.0f) {
//				t = t * -1.0f;
//			}
//
//			b = glm::normalize(glm::cross(t, n));
//		}
//
//		printf("OBJ file was successfully loaded! Number of Vertices (%i) and Indices (%i)\n", out_vertices.size(), indices.size());
//
//		return { out_vertices, indices };
//	}
//
//	static std::unordered_map<int, JointTransform> AddFirstKeyFrame(std::unordered_map<int, JointTransform>& k, Joint j) 
//	{
//		JointTransform jt = JointTransform(glm::vec3(0,0,0), glm::quat());
//		k[j.GetJointId()] = jt;
//
//		for (Joint child : j.GetChildren()) {
//			AddFirstKeyFrame(k, child);
//		}
//
//		return k;
//	}
//
//	static std::tuple<std::unordered_map<std::string, Animation>, Joint, std::vector<std::string>> LoadSkeletonAndAnimations(const char* file_path)
//	{
//		std::unordered_map<std::string, Animation> finalAnimationList;
//
//		pugi::xml_document doc;
//
//		if (!doc.load_file(file_path)) {
//			printf("ERROR! Could not open .DAE file at the path '%s'", file_path);
//			return {};
//		}
//
//		auto jointInformation = LoadSkeleton(file_path);
//		Joint root = std::get<0>(jointInformation);
//		std::vector<std::string> jointNames = std::get<1>(jointInformation);
//
//		pugi::xml_node controllers = doc.first_child().child("library_controllers");
//		std::vector<glm::mat4> invBindMatrices = GetJointInvBindMatrices(controllers);
//
//		pugi::xml_node animation_library = doc.find_node(search_id_and_name("library_animations")).child("animation");
//
//		pugi::xml_object_range<pugi::xml_node_iterator> animation_children = animation_library.children();
//
//		std::unordered_map<int, std::pair<std::vector<float>, std::vector<glm::mat4>>> animation_mapping;
//
//		// Loop through all animation children (first node is the name of animation)
//		for (auto anim : animation_children) {
//			std::string animation_name = anim.attribute("name").value();
//
//			std::string collada_name_format = animation_name;
//			std::replace(collada_name_format.begin(), collada_name_format.end(), '.', '_');
//			collada_name_format += "_";
//
//			std::string suffix = "/transform";
//
//			std::vector<KeyFrame> frames;
//			float total_abimation_length = 0.0f;
//
//			int keyframe_count = -1;
//
//			while (anim != NULL) {
//
//				std::string anim_id = anim.attribute("id").value();
//
//				std::string target = anim.find_child(search_id_and_name("channel")).attribute("target").value();
//				target.erase(target.begin(), target.begin() + collada_name_format.length());
//
//				if ("/" + target != suffix) {
//					target.erase(target.find(suffix), suffix.length());
//				}
//				else {
//					anim = anim.next_sibling();
//					continue;
//				}
//
//				if (std::find(jointNames.cbegin(), jointNames.cend(), target) == jointNames.cend()) {
//					anim = anim.next_sibling();
//					continue;
//				}
//
//				int current_joint = FindJointId(target, root);
//
//				if (current_joint >= 0) {
//
//					pugi::xml_node input = anim.find_node(search_id_and_name((anim_id + "-input-array").c_str()));
//					pugi::xml_node ouput = anim.find_node(search_id_and_name((anim_id + "-output-array").c_str()));
//
//					int count = std::stoi(input.attribute("count").value());
//
//					if (keyframe_count != count) keyframe_count = count;
//
//					std::vector<float> times;
//					std::vector<glm::mat4> matrices;
//
//					std::istringstream timestamps(input.first_child().value());
//					for (std::string s; timestamps >> s;) {
//						times.push_back(std::stof(s));
//					}
//
//					std::vector<float> ttt;
//					std::istringstream transforms(ouput.first_child().value());
//					for (std::string s; transforms >> s;) {
//						ttt.push_back(std::stof(s));
//					}
//
//					for (int i = 0; i < keyframe_count; ++i) {
//						float mat4_4[16];
//						for (int j = 0; j < 16; ++j) {
//							mat4_4[j] = ttt[i * 16 + j];
//						}
//
//						glm::mat4 matrix = glm::make_mat4(mat4_4);
//
//						matrices.push_back(matrix);
//					}
//
//					animation_mapping[current_joint] = std::make_pair(times, matrices);
//
//					anim = anim.next_sibling();
//				}
//			}
//
//
//			std::unordered_map<int, JointTransform> positions;
//			for (int i = 0; i < keyframe_count; ++i) {
//				float timeStamp = -1.0f;
//
//				for (auto element : animation_mapping) {
//					if (timeStamp < 0.0f) timeStamp = element.second.first[i];	// Get TimeStamp
//
//					glm::mat4 jointMatrix = element.second.second[i];
//					glm::vec3 transform = glm::vec3(jointMatrix[0][3], jointMatrix[1][3], jointMatrix[2][3]);
//					glm::quat rotation = glm::toQuat(glm::transpose(jointMatrix));
//
//					positions[element.first] = JointTransform(transform, rotation);
//				}
//
//				KeyFrame frame = KeyFrame(timeStamp, positions);
//				frames.push_back(frame);
//			}
//
//			float time = frames[frames.size() - 1].GetTimeStamp();
//			finalAnimationList["LookCute"] = Animation(time, frames);
//			break;
//		}
//
//		return { finalAnimationList, root, jointNames };
//	}
//
//	static Animation LoadAnimation(const char* file_path, Joint& root, const std::vector<std::string>& jointNames) 
//	{
//		pugi::xml_document doc;
//		if (!doc.load_file(file_path)) {
//			printf("ERROR! Could not open .DAE file at the path '%s'", file_path);
//			return {};
//		}
//
//		std::vector<KeyFrame> frames;
//		std::unordered_map<int, std::pair<std::vector<float>, std::vector<glm::mat4>>> animation_mapping;
//
//		std::string suffix = "/transform";
//
//		float total_abimation_length = 0.0f;
//		int keyframe_count = -1;
//
//		pugi::xml_node animation_library = doc.find_node(search_id_and_name("library_animations")).child("animation");
//
//		pugi::xml_node anim = animation_library.first_child();
//
//
//		std::string animation_name = anim.attribute("name").value();
//		std::string collada_name_format = animation_name;
//		std::replace(collada_name_format.begin(), collada_name_format.end(), '.', '_');
//		collada_name_format += "_";
//
//
//		while (anim.first_child() != NULL)
//		{
//			std::string anim_id = anim.attribute("id").value();
//
//			std::string target = anim.find_child(search_id_and_name("channel")).attribute("target").value();
//			target.erase(target.begin(), target.begin() + collada_name_format.length());
//
//			if ("/" + target != suffix) {
//				target.erase(target.find(suffix), suffix.length());
//			}
//			else {
//				anim = anim.next_sibling();
//				continue;
//			}
//
//			if (std::find(jointNames.cbegin(), jointNames.cend(), target) == jointNames.cend()) {
//				anim = anim.next_sibling();
//				continue;
//			}
//
//			int current_joint = FindJointId(target, root);
//
//			if (current_joint >= 0) {
//
//				pugi::xml_node input = anim.find_node(search_id_and_name((anim_id + "-input-array").c_str()));
//				pugi::xml_node ouput = anim.find_node(search_id_and_name((anim_id + "-output-array").c_str()));
//
//				int count = std::stoi(input.attribute("count").value());
//				if (keyframe_count != count) keyframe_count = count;
//
//				std::vector<float> times;
//				std::vector<glm::mat4> matrices;
//
//				std::istringstream timestamps(input.first_child().value());
//				for (std::string s; timestamps >> s;) {
//					times.push_back(std::stof(s));
//				}
//
//				std::vector<float> ttt;
//				std::istringstream transforms(ouput.first_child().value());
//				for (std::string s; transforms >> s;) {
//					ttt.push_back(std::stof(s));
//				}
//
//				for (int i = 0; i < keyframe_count; ++i) {
//					float mat4_4[16];
//					for (int j = 0; j < 16; ++j) {
//						mat4_4[j] = ttt[i * 16 + j];
//					}
//
//					glm::mat4 matrix = glm::make_mat4(mat4_4);
//
//					matrices.push_back(matrix);
//				}
//
//				animation_mapping[current_joint] = std::make_pair(times, matrices);
//
//				anim = anim.next_sibling();
//			}
//		}
//
//		std::unordered_map<int, JointTransform> positions;
//		for (int i = 0; i < keyframe_count; ++i) {
//			float timeStamp = -1.0f;
//
//			for (auto element : animation_mapping) {
//				if (timeStamp < 0.0f) timeStamp = element.second.first[i];	// Get TimeStamp
//
//				glm::mat4 jointMatrix = element.second.second[i];
//				glm::vec3 transform = glm::vec3(jointMatrix[0][3], jointMatrix[1][3], jointMatrix[2][3]);
//				glm::quat rotation = glm::toQuat(glm::transpose(jointMatrix));
//
//				positions[element.first] = JointTransform(transform, rotation);
//			}
//
//			KeyFrame frame = KeyFrame(timeStamp, positions);
//			frames.push_back(frame);
//		}
//
//		float time = frames[frames.size() - 1].GetTimeStamp();
//		return Animation(time, frames);
//	}
//}