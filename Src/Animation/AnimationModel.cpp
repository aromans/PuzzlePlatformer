#include "AnimationModel.h"

#include "../Renderer/Renderer.h"

#include "DaeLoader.h"

#include "src/Input.h"

namespace Engine {

	AnimationModel::AnimationModel(const std::string& obj_path, const std::string& albedo, const std::string& normal, Shader* shader)
	{
		auto parsed_obj = LoadModel(obj_path.c_str());
		m_Mesh = new Mesh(std::get<0>(parsed_obj), std::get<1>(parsed_obj));

		animator = new Animator(obj_path.c_str());

		m_Skeleton = &animator->GetSkeletonRoot();		

		m_Shader = shader;

		m_Material = new Material(shader, albedo, normal, "");
		m_Material->SetProperties(glm::vec3(.0f), .0f);

		m_VertexArray.reset(VAO::Create());

		// Index Buffer Creation
		IBO* indexBuffer;
		indexBuffer = IBO::Create(&m_Mesh->Indices()[0], m_Mesh->IndicesSize());

		// Vertex Buffer Creation
		VBO* vertexBuffer;
		vertexBuffer = VBO::Create(&m_Mesh->AnimVertices()[0], m_Mesh->AnimVerticesSize());

		std::vector<BufferElement> layout = {
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, position) },
			{ 2, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, texcoord) },
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, normal) },
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, averagedTangent) },
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, averagedBitangent) },
			{ 4, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, jointIds), GL_INT },
			{ 4, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, weights), GL_FLOAT }
		};

		vertexBuffer->AddAttributes(layout);

		m_VertexArray->AddIBO(*indexBuffer);
		m_VertexArray->AddVBO(*vertexBuffer);
	}

	AnimationModel::AnimationModel(const std::string& obj_path, const std::string& anim_path, const std::string& albedo, const std::string& normal, Shader* shader)
	{
		auto parsed_obj = LoadModel(obj_path.c_str());
		m_Mesh = new Mesh(std::get<0>(parsed_obj), std::get<1>(parsed_obj));

		animator = new Animator(anim_path.c_str());
		std::string animationFile = "ObjFiles/Slime-lookAnim.dae";
		animator->AddAnimation(animationFile, "LookAround");
		animator->AddAnimation("ObjFiles/weirdJump.dae", "Move");

		m_Skeleton = &animator->GetSkeletonRoot();

		m_Shader = shader;

		m_Material = new Material(shader, albedo, normal, "Textures/AOSlimer.png");
		m_Material->SetProperties(glm::vec3(.0f), .0f);

		m_VertexArray.reset(VAO::Create());

		// Index Buffer Creation
		IBO* indexBuffer;
		indexBuffer = IBO::Create(&m_Mesh->Indices()[0], m_Mesh->IndicesSize());

		// Vertex Buffer Creation
		VBO* vertexBuffer;
		vertexBuffer = VBO::Create(&m_Mesh->AnimVertices()[0], m_Mesh->AnimVerticesSize());

		std::vector<BufferElement> layout = {
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, position) },
			{ 2, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, texcoord) },
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, normal) },
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, averagedTangent) },
			{ 3, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, averagedBitangent) },
			{ 4, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, jointIds), GL_INT },
			{ 4, sizeof(AnimVertex), (GLvoid*)offsetof(AnimVertex, weights), GL_FLOAT }
		};

		vertexBuffer->AddAttributes(layout);

		m_VertexArray->AddIBO(*indexBuffer);
		m_VertexArray->AddVBO(*vertexBuffer);
	}

	AnimationModel::~AnimationModel()
	{

	}

	void AnimationModel::PlayAnimation(const std::string& anim_name)
	{
		animator->Play(anim_name);
	}

	void AnimationModel::Render(bool& pass)
	{
		std::vector<glm::mat4> transforms = GetJointTransforms();
		for (int i = 0; i < transforms.size(); ++i) {
			m_Shader->SetMat4f(transforms[i], ("JointTransforms[" + std::to_string(i) + "]").c_str());
		}

		m_Shader->Set1i(1, "IsAnimated");

		m_Material->AssignMaterial(pass);
		Renderer::Submit(*m_VertexArray);
	}

	void AnimationModel::Update(double dt)
	{
		// For Debugging - Making sure multiple animations work
		if (Input::IsKeyPressed(GLFW_KEY_1)) {
			PlayAnimation("LookAround");
		}
		else if (Input::IsKeyPressed(GLFW_KEY_2)) {
			PlayAnimation("LookCute");
		}
		else if (Input::IsKeyPressed(GLFW_KEY_3)) {
			PlayAnimation("Move");
		}
		else if (Input::IsKeyPressed(GLFW_KEY_0)) {
			StopAnimation();
		}

		animator->Update(dt);
	}

	std::vector<glm::mat4> AnimationModel::GetJointTransforms()
	{
		std::vector<glm::mat4> AllJointTransforms;
		AddJointsToArray(*m_Skeleton, AllJointTransforms);
		return AllJointTransforms;
	}

	void AnimationModel::AddJointsToArray(Joint current, std::vector<glm::mat4>& matrix)
	{
		glm::mat4 finalMatrix = current.GetAnimatedTransform();

		if (finalMatrix != glm::mat4())
			matrix.push_back(finalMatrix);

		for (Joint child : current.GetChildren()) {
			AddJointsToArray(child, matrix);
		}
	}

}
