#version 330 core

#define NR_POINT_LIGHTS 64
#define NR_SPOT_LIGHTS 5

struct Fog
{
	vec3 color;
	float density;
};

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

uniform DirectionalLight directionalLight;
uniform int pointCount;
uniform int spotCount;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform Fog fog;

uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
float getFogFactor(Fog fog, float fogCoordinate);
vec3 computeDiffuse(vec3 lightDir, vec3 normal, vec3 lightDiffuse);
vec3 computeSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightSpecular);
float computeAttenuation(vec3 lightPos, float constant, float linear, float quadratic);


void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 color = computeDirectionalLight(directionalLight, normal, viewDir);

	for (int i = 0; i < pointCount; i++)
	{
		color += computePointLight(pointLights[i], normal, viewDir);
	}

	for (int i = 0; i < spotCount; i++)
	{
		color += computeSpotLight(spotLights[i], normal, viewDir);
	}

	//	float fogCoordinate = length(FragPos - viewPos);
	//	FragColor = mix(vec4(color, 1.0), vec4(fog.color, 1.0), getFogFactor(fog, fogCoordinate));
		FragColor = vec4(color, 1.0);
	//	FragColor = vec4(normal, 1.0);
}


vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float attenuation = computeAttenuation(light.position, light.constant, light.linear, light.quadratic);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords)) * attenuation;
	vec3 diffuse = computeDiffuse(lightDir, normal, light.diffuse) * attenuation * intensity;
	vec3 specular = computeSpecular(lightDir, normal, viewDir, light.specular) * attenuation * intensity;

	return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	float attenuation = computeAttenuation(light.position, light.constant, light.linear, light.quadratic);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords)) * attenuation;
	vec3 diffuse = computeDiffuse(lightDir, normal, light.diffuse) * attenuation;
	vec3 specular = computeSpecular(lightDir, normal, viewDir, light.specular) * attenuation;

	return ambient + diffuse + specular;
}

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	//	vec3 lightDir = normalize(-light.direction);
	vec3 lightDir = vec3(0.0, 0.0, 0.0);
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse1, TexCoords));
	vec3 ambient = vec3(0.0, 0.0, 0.0);
	vec3 diffuse = computeDiffuse(lightDir, normal, light.diffuse);
	vec3 specular = computeSpecular(lightDir, normal, viewDir, light.specular);
	return ambient + diffuse + specular;
}

vec3 computeDiffuse(vec3 lightDir, vec3 normal, vec3 lightDiffuse)
{
	float diff = max(dot(normal, lightDir), 0.0);
	return lightDiffuse * diff * vec3(texture(material.diffuse1, TexCoords));
}

vec3 computeSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightSpecular)
{
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	return lightSpecular * spec * vec3(texture(material.specular1, TexCoords));
}

float computeAttenuation(vec3 lightPos, float constant, float linear, float quadratic)
{
	float distance = length(lightPos - FragPos);
	return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}

float getFogFactor(Fog fog, float fogCoordinate)
{
	float result = exp(-pow(fog.density * fogCoordinate, 2.0));
	return 1.0 - clamp(result, 0.0, 1.0);
}