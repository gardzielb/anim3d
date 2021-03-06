#version 330 core

#define NR_POINT_LIGHTS 1024
#define NR_SPOT_LIGHTS 5

struct Fog
{
	vec3 color;
	float density;
};

struct Material
{
	vec3 diffuse;
	float specular;
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

uniform float shininess;
uniform vec3 viewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec2 TexCoords;

out vec4 FragColor;

vec3 computeSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir, Material material);
vec3 computePointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir, Material material);
vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material material);
float getFogFactor(Fog fog, float fogCoordinate);
vec3 computeDiffuse(vec3 lightDir, vec3 normal, vec3 lightDiffuse, vec3 matDiffuse);
vec3 computeSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightSpecular, Material material);
float computeAttenuation(vec3 fragPos, vec3 lightPos, float constant, float linear, float quadratic);


void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;

	if (Normal == vec3(0.0, 0.0, 0.0))
		discard;

	vec3 normal = normalize(Normal);

	Material material;
	material.diffuse = texture(gAlbedoSpec, TexCoords).rgb;
	material.specular = texture(gAlbedoSpec, TexCoords).a;
	material.shininess = shininess;

	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 color = computeDirectionalLight(directionalLight, normal, viewDir, material);

	for (int i = 0; i < pointCount; i++)
	{
		color += computePointLight(pointLights[i], fragPos, normal, viewDir, material);
	}

	for (int i = 0; i < spotCount; i++)
	{
		color += computeSpotLight(spotLights[i], fragPos, normal, viewDir, material);
	}

	float fogCoordinate = length(fragPos - viewPos);
	FragColor = mix(vec4(color, 1.0), vec4(fog.color, 1.0), getFogFactor(fog, fogCoordinate));
}


vec3 computeSpotLight(SpotLight light, vec3 fragPos, vec3 normal, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	float attenuation = computeAttenuation(fragPos, light.position, light.constant, light.linear, light.quadratic);

	vec3 ambient = light.ambient * material.diffuse * attenuation;
	vec3 diffuse = computeDiffuse(lightDir, normal, light.diffuse, material.diffuse) * attenuation * intensity;
	vec3 specular = computeSpecular(lightDir, normal, viewDir, light.specular, material) * attenuation * intensity;

	return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float attenuation = computeAttenuation(fragPos, light.position, light.constant, light.linear, light.quadratic);

	vec3 ambient = light.ambient * material.diffuse * attenuation;
	vec3 diffuse = computeDiffuse(lightDir, normal, light.diffuse, material.diffuse) * attenuation;
	vec3 specular = computeSpecular(lightDir, normal, viewDir, light.specular, material) * attenuation;

	return ambient + diffuse + specular;
}

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material material)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 ambient = light.ambient * material.diffuse;
	vec3 diffuse = computeDiffuse(lightDir, normal, light.diffuse, material.diffuse);
	vec3 specular = computeSpecular(lightDir, normal, viewDir, light.specular, material);
	return ambient + diffuse + specular;
}

vec3 computeDiffuse(vec3 lightDir, vec3 normal, vec3 lightDiffuse, vec3 matDiffuse)
{
	float diff = max(dot(normal, lightDir), 0.0);
	return lightDiffuse * diff * matDiffuse;
}

vec3 computeSpecular(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightSpecular, Material material)
{
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	return lightSpecular * spec * material.specular * material.diffuse;
	//	return vec3(0.2, 0.2, 0.2);
}

float computeAttenuation(vec3 fragPos, vec3 lightPos, float constant, float linear, float quadratic)
{
	float distance = length(lightPos - fragPos);
	return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}

float getFogFactor(Fog fog, float fogCoordinate)
{
	float result = exp(-pow(fog.density * fogCoordinate, 2.0));
	return 1.0 - clamp(result, 0.0, 1.0);
}