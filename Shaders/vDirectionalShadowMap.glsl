#version 460

layout (location = 0) in vec3 pos;

uniform mat4 M;
uniform mat4 directionalLightTransform; // Proj * View

void main()
{
	gl_Position = directionalLightTransform * M * vec4(pos, 1.0);
}