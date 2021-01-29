#version 330 core

#define NR_POINT_LIGHTS 5
#define NR_SPOT_LIGHTS 5

struct Fog
{
	vec3 color;
	float density;
};

//struct Material
//{
//	sampler2D diffuse1;
//	sampler2D specular1;
//	float shininess;
//};

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
//uniform Fog fog;

//uniform Material material;
uniform vec3 viewPos;
uniform float shininess;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

//in vec3 FragPos;
//in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 albedo, float specular, float shininess);
vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 albedo, float specular, float shininess);
vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 albedo, float specular, float shininess);
//float getFogFactor(Fog fog, float fogCoordinate);


void main()
{
	// retrieve data from G-buffer
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
	float Specular = texture(gAlbedoSpec, TexCoords).a;

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
}


vec3 computeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 albedo, float specular, float shininess)
{
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = 0.05 * albedo * attenuation;
	vec3 diffuse = light.diffuse * diff * albedo * attenuation * intensity;
	vec3 specular = light.specular * spec * albedo * specular * attenuation * intensity;

	return ambient + diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 albedo, float specular, float shininess)
{
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = 0.05 * albedo * attenuation;
	vec3 diffuse = light.diffuse * diff * albedo * attenuation;
	vec3 specular = light.specular * spec * albedo * specular * attenuation;

	return ambient + diffuse + specular;
}

vec3 computeDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 albedo, float specular, float shininess)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = 0.05 * albedo;
	vec3 diffuse = light.diffuse * diff * albedo;
	vec3 specular = light.specular * spec * albedo * specular;
	return ambient + diffuse + specular;
}

//float getFogFactor(Fog fog, float fogCoordinate)
//{
//	float result = exp(-pow(fog.density * fogCoordinate, 2.0));
//	return 1.0 - clamp(result, 0.0, 1.0);
//}