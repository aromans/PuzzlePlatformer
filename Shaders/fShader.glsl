#version 460						 
							
in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 vPosition;
//in vec3 v_fogDepth;
		 
out vec4 color;

const int MAX_POINT_LIGHTS = 5;
const int MAX_SPOT_LIGHTS = 5;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DLight 
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
	vec3 specular;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material  material;
uniform vec3      cameraPos;

vec4 CalcLightByDirection(Light light, vec3 direction) 
{
	// Ambient Light
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	// Diffuse Light
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

	// Specular Light
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f) 
	{
		vec3 posToViewVector = normalize(cameraPos - vPosition);
		vec3 reflectedVector = normalize(reflect(direction, normalize(Normal)));

		float specularFactor = pow(max(dot(posToViewVector, reflectedVector), 0.0f), 32.0f);

		specularColor = vec4(light.color * material.specular * specularFactor, 1.0f);
	}

	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight pLight)
{
	// Calculate the direction
	vec3 direction = vPosition - pLight.position;
	float distance = length(direction);

	// Direction Vector Normalized
	direction = normalize(direction);

	vec4 color = CalcLightByDirection(pLight.base, direction);

	// ax^2 + bx + c -> x being the distance
	float attenuation = 1.0 / (pLight.exponent * distance * distance +
							   pLight.linear * distance +
							   pLight.constant);

	return (color * attenuation);
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for (int i = 0; i < pointLightCount; ++i)
	{
		totalColor += CalcPointLight(pointLights[i]);
	}

	return totalColor;
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(vPosition - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);

	if (slFactor > sLight.edge) 
	{
		vec4 color = CalcPointLight(sLight.base);
		return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
	} else {
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for (int i = 0; i < spotLightCount; ++i)
	{
		totalColor += CalcSpotLight(spotLights[i]);
	}

	return totalColor;
}

float getFogFactor(float d) 
{
	const float max = 20.0;
	const float min = 10.0;

	if (d >= max) return 1;
	if (d <= min) return 0;

	return 1 - (max - d) / (max - min);
}

const float fogNear = 8.f;
const float fogFar = 14.f;
									 
void main() 
{
	// Direction Light
	vec4 finalColor = CalcDirectionalLight();

	// Point Lights
	finalColor += CalcPointLights();

	// Spot Lights
	finalColor += CalcSpotLights();
	
	// -- Fog Stuff --
	// Linear Fog
	//float distance = distance(cameraPos, vPosition);
	//float fogFactor = getFogFactor(distance);
	// SmoothStep Fog w/ Near & Far
	//float fogDepthAmnt = length(v_fogDepth);
	//float fogFactor = smoothstep(fogNear, fogFar, fogDepthAmnt);
	// Fog Color
	//vec4 fogColor = vec4(0.3f, .37f, .44f, 1.f); // End -- Fog Stuff --

	vec4 fragColor = texture(theTexture, TexCoord) * finalColor;
	
	if (fragColor.a < 0.15)
		discard;

	color = fragColor;

	//color = mix(color, fogColor, fogFactor); // Uncomment to incorporate Fog
}