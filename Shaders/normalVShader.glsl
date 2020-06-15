#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_Tangent;
out vec3 v_Bitangent;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main()
{
	gl_Position = MVP * vec4(pos, 1.0);

	v_Position = (M * vec4(pos, 1.0)).xyz;

	v_TexCoord = texcoord;

	v_Tangent = (M * vec4(tangent, 1.0)).xyz;

	v_Bitangent = (M * vec4(bitangent, 1.0)).xyz;

	v_Normal = (M * vec4(normal, 1.0)).xyz;
}