#version 460

const int MAX_POINT_LIGHTS = 5;
const int MAX_SPOT_LIGHTS = 5;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	int has_normal_map;

	sampler2D diffuse;
	sampler2D normal;
	vec3 specular;
	float shininess;
};

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_Tangent;
in vec3 v_Bitangent;

out vec4 out_color;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

// Lighting Uniforms
uniform int pointLightCount;
uniform int spotLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform DirectionalLight directionalLight;

uniform Material material;
uniform vec3 cameraPos;

vec4 CalcLight(Light light, vec3 direction, vec3 normal)
{
	vec4 ambient = vec4(light.color, 1.0f) * light.ambientIntensity;
	float diffuseFactor = max(dot(normalize(direction), normalize(normal)), 0.0f);

	vec4 diffuse = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

	vec4 specular = vec4(0, 0, 0, 0);
	if (diffuseFactor > 0.0f)
	{ 
		vec3 VertexToEye = normalize(cameraPos - v_Position);
		vec3 Reflection = normalize(reflect(-direction, normalize(normal)));

		float specularFactor = dot(Reflection, VertexToEye);

		if (specularFactor > 0.0f)
		{
			specularFactor = pow(clamp(specularFactor, 0.0f, 1.0f), material.shininess);
			specular = vec4(light.color * material.specular * specularFactor, 1.0f);
		}
	}

	return (ambient + diffuse + specular);
}

vec4 CalcPointLight(PointLight light, vec3 normal)
{
	vec3 lightDir = v_Position - light.position;
	float distance = length(lightDir);
	lightDir = normalize(lightDir);

	vec4 Color = CalcLight(light.base, lightDir, normal);
	float attenuation = light.constant + 
						(light.linear * distance) +
						(light.exponent * distance * distance);

	return Color / attenuation;
}

vec4 CalcSpotLight(SpotLight light, vec3 normal)
{
	vec3 direction = normalize(v_Position - light.base.position);
	float lightFactor = dot(light.direction, direction);

	if (lightFactor > light.edge) { 
		vec4 Color = CalcPointLight(light.base, normal);
		return Color * (1.0f - (1.0f - lightFactor) * 1.0f / (1.0f - light.edge));
	} else {
		return vec4(0,0,0,0);
	}
}

vec3 CalcBumpedNormal()
{
	if (material.has_normal_map == 0) return normalize(v_Normal);

	vec3 Normal = normalize(v_Normal);
	vec3 Tangent = normalize(v_Tangent);
	vec3 Bitangent = normalize(v_Bitangent);
	vec3 BumpMapNormal = (texture(material.normal, v_TexCoord)).xyz;
	vec3 NewNormal;
	mat3 TBN = mat3(Tangent, Bitangent, Normal);
	NewNormal = TBN * BumpMapNormal;
	NewNormal = normalize(NewNormal);
	return NewNormal;
}

void main()
{
	vec3 Normal = CalcBumpedNormal();

	vec4 light_color = CalcLight(directionalLight.base, directionalLight.direction, Normal);

	for (int i = 0; i < pointLightCount; ++i)
	{
		light_color += CalcPointLight(pointLights[i], Normal);
	}

	for (int i = 0; i < spotLightCount; ++i) 
	{
		light_color += CalcSpotLight(spotLights[i], Normal);
	}

	vec4 frag_color = texture(material.diffuse, v_TexCoord);

	if (frag_color.a < 0.5)
		discard;

	out_color = texture(material.diffuse, v_TexCoord) * light_color;
}