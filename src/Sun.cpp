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

	float intensity = std::max( 0.0f, std::sin( glm::radians( currentAngle ) ) );
//	float sqIntensity = intensity * intensity;
//	light.diffuse = intensity * fullLight.diffuse;
//	light.specular = intensity * fullLight.specular;
}

void Sun::setInShaderTypeSpecific( const Shader & shader, const std::string & sourceName ) const
{
	glm::mat4 rotationMatrix = glm::rotate(
			glm::mat4( 1.0f ), glm::radians( currentAngle ), rotationAxis
	);
	glm::vec4 pos = rotationMatrix * glm::vec4( glm::vec3( -1.0f, 0.0f, 0.0f ), 1.0f );
	shader.setVector( sourceName + ".direction", -glm::vec3( pos ) );
}

Sun::Sun( const Light & light, float rotationAngle )
		: LightSource( light ), fullLight( light ), rotationAngle( rotationAngle ), rotationAxis( 0.0f, 0.0f, 1.0f )
{}
