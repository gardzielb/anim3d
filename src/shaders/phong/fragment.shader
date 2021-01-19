#version 330 core

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

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
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);


void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 color = computeDirectionalLight(directionalLight, normal, viewDir);

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		color += computePointLight(pointLights[i], normal, viewDir);
	}

	for (int i = 0; i < NR_SPOT_LIGHTS; i++)
	{
		color += computeSpotLight(spotLights[i], normal, viewDir);
	}

	FragColor = vec4(color, 1.0);
}


vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse1, TexCoords)) * attenuation;
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords)) * attenuation * intensity;
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords)) * attenuation * intensity;

	return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse1, TexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, TexCoords));
	return ambient + diffuse + specular;
}