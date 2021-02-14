//
// Created by bartosz on 1/22/21.
//

#include "Sun.h"
#include <glm/ext/matrix_transform.hpp>
#include <spdlog/spdlog.h>


void Sun::move()
{
	float rotationAngle = 180.0f / dayLength;

	currentAngle += rotationAngle;
	if ( currentAngle >= 360 )
		currentAngle -= 360;

	brightness = computeBrightness();
}

void Sun::setInShader( const Shader & shader ) const
{
	light.diffuse = brightness * fullLight.diffuse;
	light.specular = brightness * fullLight.specular;

	light.setInShader( shader, "directionalLight" );
	glm::mat4 rotationMatrix = glm::rotate(
			glm::mat4( 1.0f ), glm::radians( currentAngle ), rotationAxis
	);
	glm::vec4 pos = rotationMatrix * glm::vec4( glm::vec3( -1.0f, 0.0f, 0.0f ), 1.0f );
	glm::vec3 dir = -glm::vec3( pos );
	shader.setVector( "directionalLight.direction", dir );
}

Sun::Sun( const Light & light, float dayLength )
		: light( light ), fullLight( light ), dayLength( dayLength ), rotationAxis( 0.0f, 0.0f, 1.0f ),
		  brightSkyColor( 0.5f, 0.75f, 0.9f ), darkSkyColor( 0.0f, 0.0f, 0.15f )
{
	zeroLight.diffuse = glm::vec3( light.diffuse.r * 0.8f, light.diffuse.g * 0.2f, light.diffuse.b * 0.2f );
	zeroLight.specular = glm::vec3( light.specular.r * 0.8f, light.specular.g * 0.2f, light.specular.b * 0.2f );
	zeroLight.ambient = light.ambient;
}

glm::vec3 Sun::getSkyColor() const
{
	return darkSkyColor + brightness * (brightSkyColor - darkSkyColor);
}

float Sun::computeBrightness() const
{
	return std::max( 0.0, 0.022222 * currentAngle - 0.000123 * currentAngle * currentAngle );
}
