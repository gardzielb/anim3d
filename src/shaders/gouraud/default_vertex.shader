#version 330 core

#define NR_POINT_LIGHTS 64
#define NR_SPOT_LIGHTS 5

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
	float shininess;
};

struct Fog
{
	vec3 color;
	float density;
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

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform DirectionalLight directionalLight;
uniform int pointCount;
uniform int spotCount;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform Fog fog;
uniform Material material;
uniform vec3 viewPos;

out vec3 color;

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 vPos);
vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vPos);
vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 vPos);
float getFogFactor(Fog fog, float fogCoordinate);


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vec3 normal = normalize(transpose(inverse(mat3(model))) * aNormal);
	vec3 vPos = vec3(model * vec4(aPos, 1.0));
	vec3 viewDir = normalize(viewPos - vPos);

	color = computeDirectionalLight(directionalLight, normal, viewDir, vPos);

	for (int i = 0; i < pointCount; i++)
	{
		color += computePointLight(pointLights[i], normal, viewDir, vPos);
	}

	for (int i = 0; i < spotCount; i++)
	{
		color += computeSpotLight(spotLights[i], normal, viewDir, vPos);
	}

	float fogCoordinate = length(vPos - viewPos);
	color = mix(color, fog.color, getFogFactor(fog, fogCoordinate));
}


vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 vPos)
{
	vec3 lightDir = normalize(light.position - vPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - vPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse1, aTexCoords)) * attenuation;
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse1, aTexCoords)) * attenuation * intensity;
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, aTexCoords)) * attenuation * intensity;

	return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 vPos)
{
	vec3 lightDir = normalize(light.position - vPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - vPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient  * vec3(texture(material.diffuse1, aTexCoords));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(material.diffuse1, aTexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, aTexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 vPos)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse1, aTexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse1, aTexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular1, aTexCoords));
	return ambient + diffuse + specular;
}

float getFogFactor(Fog fog, float fogCoordinate)
{
	float result = exp(-pow(fog.density * fogCoordinate, 2.0));
	return 1.0 - clamp(result, 0.0, 1.0);
}