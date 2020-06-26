#pragma once

#include "../Model.h"
#include "Joint.h"

#include "Animator.h"

namespace Engine {

	class AnimationModel : public Model
	{
	public:
		AnimationModel() {}
		AnimationModel(const std::string& obj_path, const std::string& albedo, const std::string& normal, Shader* shader);
		AnimationModel(const std::string& obj_path, const std::string& anim_path, const std::string& albedo, const std::string& normal, Shader* shader);
		virtual ~AnimationModel() override;

		void PlayAnimation(const std::string& anim_name);
		void StopAnimation() { animator->Stop(); }

		virtual void Render(bool& pass) override;

		void Update(double dt);

		std::vector<glm::mat4> GetJointTransforms();

		Mesh& GetMesh() const { return *m_Mesh; }

	private:
		void AddJointsToArray(Joint current, std::vector<glm::mat4>& matrix);

	private:
		std::unique_ptr<VAO> m_VertexArray;
		const Joint* m_Skeleton;
		Material* m_Material;
		Mesh* m_Mesh;
		Shader* m_Shader;

		Animator* animator;
	};

}

