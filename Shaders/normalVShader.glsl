#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

/*out vec3 v_Position;
out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;
out vec3 v_PassedNormal;

out vec3 E;*/

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

void main()
{
	gl_Position = MVP * vec4(pos, 1.0);

	vs_out.v_Position = (M * vec4(pos, 1.0)).xyz;

	vs_out.v_Normal = mat3(NormalMatrix) * normal;

	vs_out.v_TexCoord = texcoord;

	vec3 N = normalize((NormalMatrix * vec4(normal, 0.0)).xyz);
	vec3 T = normalize((NormalMatrix * vec4(tangent, 0.0)).xyz);
	vec3 B = normalize((NormalMatrix * vec4(bitangent, 0.0)).xyz);

	vs_out.TBN = mat3(T, B, N);

	DirectionalLightSpacePos = directionalLightTransform * M * vec4(pos, 1.0);
}