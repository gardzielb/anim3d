//
// Created by bartosz on 1/22/21.
//

#include "Sun.h"
#include <glm/ext/matrix_transform.hpp>
#include <spdlog/spdlog.h>


void Sun::move()
{
	currentAngle += rotationAngle;
	if ( currentAngle >= 360 )
		currentAngle -= 360;

//	float intensity = std::max( 0.0, 0.022222 * currentAngle - 0.000123 * currentAngle * currentAngle );
//	spdlog::info( "intensity = {}", intensity );

//	light.diffuse = intensity * fullLight.diffuse;
//	light.specular = intensity * fullLight.specular;
}

void Sun::setInShader( const Shader & shader ) const
{
	light.setInShader( shader, "directionalLight" );
	glm::mat4 rotationMatrix = glm::rotate(
			glm::mat4( 1.0f ), glm::radians( currentAngle ), rotationAxis
	);
	glm::vec4 pos = rotationMatrix * glm::vec4( glm::vec3( -1.0f, 0.0f, 0.0f ), 1.0f );
	glm::vec3 dir = -glm::vec3( pos );
	shader.setVector( "directionalLight.direction", dir );
}

Sun::Sun( const Light & light, float rotationAngle )
		: light( light ), fullLight( light ), rotationAngle( rotationAngle ), rotationAxis( 0.0f, 0.0f, 1.0f )
{}
