#version 460
									     
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 vPosition;

//out vec3 v_fogDepth;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 inverseTModel;
uniform mat4 projection;
									     
void main()
{
	gl_Position = MVP * vec4(pos, 1.0);
	vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

	TexCoord = tex;

	Normal = mat3(inverseTModel) * norm;

	vPosition = (M * vec4(pos, 1.0)).xyz;

	// Depth for SmoothStep Fog
	//v_fogDepth = (V * vec4(pos, 1.0)).xyz;
}