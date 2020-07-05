//#pragma once
//
//#include "Epch.h"
//#include "Engine/Parsers/pugixml-1.10/pugixml.hpp"
//
//namespace Engine {
//
//	struct search_id_and_name1
//	{
//		const char* key;
//		search_id_and_name1(const char* k) : key(k) {}
//
//		bool operator()(pugi::xml_node node) const
//		{
//			return !strcmp(node.attribute("id").value(), key) || !strcmp(node.name(), key);
//		}
//	};
//
//	struct search_attribute_value1
//	{
//		const char* key;
//		const char* value;
//		search_attribute_value1(const char* k, const char* v) : key(k), value(v) {}
//
//		bool operator()(pugi::xml_node node) const
//		{
//			return !strcmp(node.attribute(key).value(), value);
//		}
//	};
//
//	static void LoadGeometry(pugi::xml_node geom)
//	{
//		std::string mesh_id = geom.find_node(search_id_and_name1("geometry")).attribute("id").value();
//
//		std::string tex_coord_id = geom.find_node(search_attribute_value1("semantic", "TEXCOORD")).attribute("source").value();
//		tex_coord_id.erase(std::remove(tex_coord_id.begin(), tex_coord_id.end(), '#'), tex_coord_id.end());
//
//		auto positions_array  = geom.find_node(search_attribute_value1("id", (mesh_id + "-positions-array").c_str())).first_child().value();
//		auto normals_array	  = geom.find_node(search_attribute_value1("id", (mesh_id + "-normals-array").c_str())).first_child().value();
//		auto texcoord_array	  = geom.find_node(search_attribute_value1("id", (tex_coord_id + "-array").c_str())).first_child().value();
//		auto faces_array	  = geom.find_node(search_id_and_name1("p")).first_child().value();
//
//	}
//
//	static Joint LoadJointHierarchy(Joint& node, pugi::xml_node currNode, std::vector<std::string> names)
//	{
//		for (pugi::xml_node child : currNode.children())
//		{
//			if (!strcmp(child.name(), "node"))
//			{
//				auto name = child.attribute("name").value();
//				auto name_it = std::find(names.cbegin(), names.cend(), name);
//				if (name_it != names.cend()) {
//					auto yes = child.first_child().first_child().value();
//					int index = std::distance(names.cbegin(), name_it);
//					Joint childJoint = Joint(index, name);
//					LoadJointHierarchy(childJoint, child, names);
//					node.AddChild(childJoint);
//				}
//			}
//		}
//
//		return node;
//	}
//
//	static void LoadJoints(pugi::xml_node geom)
//	{
//		std::string controller_id = geom.find_node(search_id_and_name1("controller")).attribute("id").value();
//
//		auto joints_array = geom.find_node(search_attribute_value1("id", (controller_id + "-joints-array").c_str())).first_child().value();
//		auto invBindPoses_array = geom.find_node(search_attribute_value1("id", (controller_id + "-bind_poses-array").c_str())).first_child().value();
//		auto weights_array = geom.find_node(search_attribute_value1("id", (controller_id + "-weights-array").c_str())).first_child().value();
//		auto weightAmnt_array = geom.find_node(search_id_and_name1("vcount")).first_child().value();
//		auto jointWeight_array = geom.find_node(search_id_and_name1("v")).first_child().value();
//
//		pugi::xml_node hierarchy_root = geom.find_node(search_id_and_name("library_visual_scenes")).child("visual_scene");
//
//		std::vector<std::string> joint_names;
//		std::istringstream namestream(joints_array);
//		for (std::string s; namestream >> s; joint_names.push_back(s)) { }
//		Joint root = Joint(0, joint_names[0]);
//		root = LoadJointHierarchy(root, hierarchy_root, joint_names);
//	}
//
//
//	static void LoadSkeletonAndAnimations(pugi::xml_node geom)
//	{
//		pugi::xml_node animation_root = geom.find_node(search_id_and_name("library_animations")).child("animation").first_child();
//
//		while (animation_root != NULL) 
//		{
//
//
//
//		}
//	}
//
//	static void ImportColladaFile(const char* path) 
//	{
//		pugi::xml_document doc;
//
//		if (!doc.load_file(path)) {
//			printf("ERROR reading the Collada file at path '%s'\n", path);
//			return;
//		}
//		pugi::xml_node geom = doc.first_child();
//
//		LoadJoints(geom);
//		LoadGeometry(geom);
//	}
//
//}