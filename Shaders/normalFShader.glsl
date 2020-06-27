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
	int has_ao_map;

	sampler2D diffuse;
	sampler2D normal;
	sampler2D ao;
	vec3 specular;
	float shininess;
};


in VS_OUT
{
	vec3 v_Position;	// World
	vec3 v_Normal;		// World
	vec2 v_TexCoord;	// World
	
	mat3 TBN;			// Tangent Space
} fs_in;

in vec4 DirectionalLightSpacePos;

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

// Shadow Map
uniform sampler2D directionalShadowMap;

uniform Material material;
uniform vec3 cameraPos;


float CalcDirectionalShadowFactor(vec3 direction, vec3 normal)
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float current = projCoords.z;

	vec3 Normal = normalize(normal);
	vec3 lightDir = normalize(direction);

	float bias = max(0.05 * (1.0 - dot(lightDir, Normal)), 0.005);

	float shadow = 0.0f;

	int shadowDetail = 1;

	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 9;

	if (projCoords.z > 1.0) {
		shadow = 0.0;
	}

	return shadow;
}

//if (dot(toEyePos, normalize(normal)) <  dot(normalize(direction), normalize(normal)) * 0.5) {
//			return vec4(0.0, 0.0, 0.0, 1.0);
//} 

vec4 CalcLight(Light light, vec3 direction, vec3 normal, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;
	
	if (material.has_ao_map == 1) {
		ambientColor *= texture(material.ao, fs_in.v_TexCoord).r;
	}

	float diffuseFactor = max(dot(normalize(direction), normalize(normal)), 0.0);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0)
	{
		diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0);

		vec3 toEyePos = normalize(cameraPos - fs_in.v_Position);
		vec3 reflection = normalize(reflect(-normalize(direction), normalize(normal)));

		float specularFactor = dot(reflection, toEyePos);
		
		if (dot(toEyePos, normalize(normal)) <  mix(0.05, 0.3, dot(normalize(direction), normalize(normal)))) {
			return vec4(0.0, 0.0, 0.0, 1.0);
		} 
		else if (specularFactor > 0.0)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specular * specularFactor, 1.0);
		} 
	}

	return (ambientColor + ((1.0 - shadowFactor) * (diffuseColor + specularColor)));
}

vec4 CalcPointLight(PointLight light, vec3 normal)
{
	vec3 toLightDir = fs_in.v_Position - light.position;
	float distance = length(toLightDir);
	toLightDir = normalize(toLightDir);

	vec4 final_color = CalcLight(light.base, toLightDir, normal, 0.0);

	float attenuation = light.constant +
						(light.linear * distance) +
						(light.exponent * distance * distance);

	return final_color / attenuation;
}

vec4 CalcSpotLight(SpotLight light, vec3 normal)
{
	vec3 toLightDir = normalize(fs_in.v_Position - light.base.position);
	float lightFactor = dot(normalize(light.direction), toLightDir);

	if (lightFactor > light.edge) {
		vec4 final_color = CalcPointLight(light.base, normal);
		return final_color * (1.0f - (1.0f - lightFactor) * 1.0f / (1.0f - light.edge));
	} else { 
		return vec4(0, 0, 0, 0);
	}
}


vec3 CalcNewNormal()
{
	if (material.has_normal_map == 0) return fs_in.v_Normal;

	vec3 NewNormal = texture( material.normal, fs_in.v_TexCoord ).rgb;
	NewNormal = normalize(NewNormal * 2.0 - 1.0);
	NewNormal = normalize(fs_in.TBN * NewNormal);	// Converts to World

	return NewNormal;
}

void main()
{
	vec3 Normal = CalcNewNormal();

	float shadowFactor = CalcDirectionalShadowFactor(directionalLight.direction, Normal);

	vec4 light_color = CalcLight(directionalLight.base, directionalLight.direction, Normal, shadowFactor);

	for (int i = 0; i < pointLightCount; ++i) {
		light_color += CalcPointLight(pointLights[i], Normal);
	}

	for (int i = 0; i < spotLightCount; ++i) {
		light_color += CalcSpotLight(spotLights[i], Normal);
	}

	vec4 frag_color = texture(material.diffuse, fs_in.v_TexCoord);

	if (frag_color.a < 0.5)
		discard;

	out_color = frag_color * light_color;
}