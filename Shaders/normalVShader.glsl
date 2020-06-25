#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in ivec4 jointIds;
layout (location = 6) in vec4 weights;

const int MAX_JOINTS = 150;
const int MAX_WEIGHT_JOINTS = 4;

out VS_OUT
{
	vec3 v_Position;
	vec3 v_Normal;
	vec2 v_TexCoord;

	mat3 TBN;
} vs_out;

out vec4 DirectionalLightSpacePos;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat4 NormalMatrix;	// transpose(inverse(M))
uniform mat4 directionalLightTransform;
uniform vec3 cameraPos;

uniform int IsAnimated;

uniform mat4 BindShapeMatrix;
uniform mat4 JointTransforms[MAX_JOINTS];

void main()
{
	vec4 localPosition;
	vec4 localNormal;
	vec4 localTangent;
	vec4 localBitangent;
	if (IsAnimated == 1) {
		mat4 boneTransform = JointTransforms[jointIds[0]] * weights[0];
		boneTransform += JointTransforms[jointIds[1]] * weights[1];
		boneTransform += JointTransforms[jointIds[2]] * weights[2];
		boneTransform += JointTransforms[jointIds[3]] * weights[3];
		localPosition = boneTransform * vec4(pos, 1.0);
		localNormal = boneTransform * vec4(normal, 0.0);
		localTangent = boneTransform * vec4(tangent, 0.0);
		localBitangent = boneTransform * vec4(bitangent, 0.0);
	} else {
		localPosition = vec4(pos, 1.0);
		localNormal = vec4(normal, 0.0);
		localTangent = vec4(tangent, 0.0);
		localBitangent = vec4(bitangent, 0.0);
	}


	gl_Position = MVP * localPosition;

	vs_out.v_Position = (M * localPosition).xyz;

	vs_out.v_Normal = mat3(NormalMatrix) * localNormal.xyz;

	vs_out.v_TexCoord = texcoord;

	vec3 N = normalize((NormalMatrix * localNormal).xyz);
	vec3 T = normalize((NormalMatrix * localTangent).xyz);
	vec3 B = normalize((NormalMatrix * localBitangent).xyz);

	vs_out.TBN = mat3(T, B, N);

	DirectionalLightSpacePos = directionalLightTransform * M * localPosition;
}