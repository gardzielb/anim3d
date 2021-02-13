#version 330 core

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
};

out vec4 FragColor;
in vec2 TexCoords;

uniform Material material;
uniform float brightness;

void main()
{
	FragColor = brightness * texture(material.diffuse1, TexCoords);
}
