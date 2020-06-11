#version 460						 
							
in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 vPosition;
		 
out vec4 color;

const int MAX_POINT_LIGHTS = 5;

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

struct Material
{
	vec3 specular;
};

uniform int pointLightCount;

uniform DLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

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

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for (int i = 0; i < pointLightCount; ++i)
	{
		// Calculate the direction
		vec3 direction = vPosition - pointLights[i].position;
		float distance = length(direction);

		// Direction Vector Normalized
		direction = normalize(direction);

		vec4 color = CalcLightByDirection(pointLights[i].base, direction);

		// ax^2 + bx + c -> x being the distance
		float attenuation = 1.0 / (pointLights[i].exponent * distance * distance +
								   pointLights[i].linear * distance +
							       pointLights[i].constant);

		totalColor += color * attenuation;
	}

	return totalColor;
}
									 
void main() 
{
	// Direction Light
	vec4 finalColor = CalcDirectionalLight();

	// Point Lights
	finalColor += CalcPointLights();

	color = texture(theTexture, TexCoord) * finalColor;
}